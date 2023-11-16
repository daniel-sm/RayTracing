/*
Instalar sdl2 no Ubuntu:
sudo apt-get install libsdl2-dev

Compilar sdl2 no Ubuntu:
g++ main.cpp -o main.out `sdl2-config --cflags --libs`

Compilar sdl2 no Windows
g++ main.cpp -o main.exe -I "C:\MinGW\include\SDL2" -lmingw32 -lSDL2main -lSDL2
*/

#include <SDL.h>
#include "include/cena.hpp"
#include "include/objetos.hpp"
#include "include/intersecoes.hpp"
#include "include/transformacoes.hpp"
#include "include/basics.hpp"

double raycast (Lista<Objeto> &cenario, Raio raio, Objeto* &atingido)
{
	// Vai guardar o menor valor de t 
	double menor_t = -1;

	// Percorrendo os objetos da cena
	for (auto obj : cenario)
	{
        // Guardando o retorno da intersecao para comparar com menor_t
        double t_int = obj->intersecao(raio);

        // Computando o menor valor de menor_t
        if (t_int > 0) 
        {
            if (menor_t > 0) 
            { 
                if (t_int < menor_t) 
                { 
                    menor_t = t_int; 
                    atingido = obj; 
                }
            } 
            else { menor_t = t_int; atingido = obj; }
        }
    }
    return menor_t;
}

int main(int argc, char** argv) 
{
	// Funcao que define os vertices, arestas e faces do cubo
	Cena::definirMalha();
	// Funcao que define a lista de objetos da cena
	Cena::definirCena();
	// Funcao que define a lista de fontes da cena
	Cena::definirFontes();

	// Projecao (1 - perspectiva), (2 - ortografica), (3 - obliqua) 
	int projecao = 1;
	Vetor direcaoRaio = { 0.2, 0, -1 };

	// Informacoes da Janela ***************************************************
	double xminJanela = -30; // Valor do lado esquerdo da janela em cm
	double xmaxJanela = 30; // Valor do lado direito da janela em cm
	double yminJanela = -30; // Valor da parte de cima da janela em cm
	double ymaxJanela = 30; // Valor da parte de baixo da janela em cm
	double dJanela = 80; // Distancia da janela em cm
	// Objeto da Janela
	Janela janela (xminJanela, xmaxJanela, yminJanela, ymaxJanela, dJanela); 

	// Informacoes do Canvas ***************************************************
	int nCol = 700; // Numero de colunas da grade do canvas
	int nLin = 700; // Numero de linhas da grade do canvas

	// Delta X e Y dos quadrados da grade do canvas ****************************
	double Dx = janela.getWidth() / nCol;
	double Dy = janela.getHeight() / nLin;

	// Informacoes da Camera ***************************************************
	// Vista de Cima
	Ponto eye = { 50, 200, 50 };
	Ponto at = { 50, 0, 50 };
	Ponto up = { 0, 0, 0 };
	// Vista em Diagonal
	// Ponto eye = { 150, 100, 150 };
	// Ponto at = { 0, 0, 0 };
	// Ponto up = { 0, 200, 0 };
	// Objeto da Camera 
	Camera camera (eye, at, up);
	// Transformando de Mundo para Camera
	camera.toCamera(Cena::cenario, Cena::fontes);

	// Matriz de cores *********************************************************
	Cor** cores = new Cor*[nLin];
	// Inicializando cada linha da matriz
	for (int i = 0; i < nLin; ++i) { cores[i] = new Cor[nCol]; }

	// Matriz de objetos atingidos *********************************************
	// Array de arrays de ponteiro para Objeto
	Objeto*** objetos_atingidos = new Objeto**[nLin];
	// Inicializando cada linha da matriz
	for (int i = 0; i < nLin; ++i) { objetos_atingidos[i] = new Objeto*[nCol]; }

	// Maior intensidade de cor ************************************************
	// Usada para reprocessar as cores com base na maior
	double maiorCor = 1.0; 

	// Loop de Ray Casting *****************************************************
	// Lancando raios para cada quadrado na grade do canvas
	//
	// Percorre as linhas da grade do canvas
	for (int lin = 0; lin < nLin; ++lin) 
	{
		// Coordenada Y do centro do quadriculo no frame
		double y = janela.getYMax() - (Dy / 2) - (lin * Dy);

		// Percorre as colunas da grade do canvas
		for (int col = 0; col < nCol; ++col) 
		{
			// Coordenada X do centro do quadriculo no frame
			double x = janela.getXMin() + (Dx / 2) + (col * Dx);

			// Ponto do centro do quadrado do frame
			Ponto pontoJanela { x, y, -janela.getDistance() };
			// Raio que vai ser lancado, dependendo da projecao
			Raio raio = Raio({ 0, 0, 0 }, pontoJanela); // Por padrao é perspectiva

			// Gerando Raio lancado pela janela
			switch (projecao)
			{
			case 1: // Caso seja persperctiva
				raio = Raio({ 0, 0, 0 }, pontoJanela);
				break;
			case 2: // Caso seja ortografica
				raio = Raio(pontoJanela, Vetor{0, 0, -1});
				break;
			case 3: // Caso seja obliqua
				raio = Raio(pontoJanela, direcaoRaio);
				break;
			default: 
				break;
			}
			// Armazena o objeto intersectado mais proximo 
			Objeto* atingido = nullptr; 
			// Armazena o valor de t que intersecta o objeto mais proximo
			double t_int = raycast(Cena::cenario, raio, atingido);

			// Verificando de atingiu algum objeto
			if (t_int > 0) 
			{
				// Obtendo o ponto de intersecao a partir do t encontrado
                Ponto p_int = raio.pontoIntersecao(t_int);

				// Vetor de intensidade da luz no ponto intersectado
				Vetor I { 0.0, 0.0, 0.0 };
				// Adicionando a luz ambiente
				I = I + (Cena::luzAmbiente * (atingido->material.ka));

				// Percorrendo as fontes de luz da cena
				for (auto fonte : Cena::fontes)
				{
					// Checa se o ponto esta na sombra
					if (fonte->sombra(p_int, Cena::cenario, raycast)) 
					{
						Vetor normal = atingido->obterNormal(p_int);

						// Armazena a intensidade no ponto para a fonte atual
						Vetor aux = fonte->iluminacao(
							normal,
							p_int,
							raio.getDirecao(),
							atingido->material
						);
						// Soma a intensidade de cada fonte
						I = I + aux;
					}
				}
				// Computando a maior intensidade de cor
				if (I.x > maiorCor) maiorCor = I.x;
				if (I.y > maiorCor) maiorCor = I.y;
				if (I.z > maiorCor) maiorCor = I.z;

				Cor cor { 255, 255, 255 };

				// Calcular a cor em RGB
				cor.r = (cor.r * I.x);
				cor.g = (cor.g * I.y);
				cor.b = (cor.b * I.z);

				cores[lin][col] = cor;
				objetos_atingidos[lin][col] = atingido;
			} else { 
				cores[lin][col] = { 0, 0, 0 };
				objetos_atingidos[lin][col] = nullptr;
			}
		}
	}

	// Reprocessar as cores com base na maior intensidade de cor
	for (int l = 0; l < nLin; ++l) {
		for (int c = 0; c < nCol; ++c) {
			// Arredondando para valor inteiro 
			cores[l][c].r = round(cores[l][c].r / maiorCor);
			cores[l][c].g = round(cores[l][c].g / maiorCor);
			cores[l][c].b = round(cores[l][c].b / maiorCor);
		}
	}

    // ##############################################################
    // ########################### SDL ##############################
    // ##############################################################

	// Inicializando a biblioteca para poder usar suas funções
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("Não foi possível inicializar o SDL! SDL_Error: %s", SDL_GetError());
		return 1;
	}
  
	// Criando uma janela
	SDL_Window* window = SDL_CreateWindow(
		"Computação Gráfica",   // Título da Janela
		SDL_WINDOWPOS_CENTERED, // Posição inicial X
		SDL_WINDOWPOS_CENTERED, // Posição inicial Y
		nCol,                   // Largura da janela em pixels
		nLin,                   // Altura da janela em pixels
		SDL_WINDOW_SHOWN        // Flags
	);
  
	// Verificando se a janela foi criada corretamente
	if (!window) {
		SDL_Log("Criação da janela falhou! SDL_Error: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}
  
	// Criar o objeto renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	// Verificando se o renderer foi criada corretamente
	if (!renderer) {
		SDL_Log("Criação do renderer falhou! SDL_Error: %s", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// Controle de eventos
	SDL_Event event;
	// Controle de loop
	bool isRunning = true;

	// Main loop 
	while (isRunning) {
		// Checando eventos
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_QUIT:
				isRunning = false; 
				break;
			case SDL_MOUSEBUTTONDOWN:
				for (int i = 0; i < nLin; ++i)
					for (int j = 0; j < nCol; ++j) 
						if (objetos_atingidos[event.button.y][event.button.x] == objetos_atingidos[i][j])
							cores[i][j] = { 0, 0, 0 };
				break;
			default:
				break;
			}
		}
		// Limpando o renderer
		SDL_RenderClear(renderer);
		
		for (int i = 0; i < nLin; ++i) {
			for (int j = 0; j < nCol; ++j) {
				// Designando a cor que vai pintar
				SDL_SetRenderDrawColor(
					renderer, 
					cores[i][j].r, 
					cores[i][j].g, 
					cores[i][j].b, 
					SDL_ALPHA_OPAQUE
				);
				// Pintando o pixel
				SDL_RenderDrawPoint(renderer, j, i);
			}
		}
		// Atualizando a janela com o renderer
		SDL_RenderPresent(renderer);
	}
	// Destruindo os objetos criados
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	// Desalocando cada linha da matriz de cores
	for (int i = 0; i < nLin; ++i) { delete cores[i]; }
	delete[] cores; // Desaloca a matriz por completo

	return 0;
}