/*
Instalar sdl2 no Ubuntu:
sudo apt-get install libsdl2-dev

Compilar sdl2 no Ubuntu:
g++ main.cpp -o main.out `sdl2-config --cflags --libs`

Compilar sdl2 no Windows
g++ main.cpp -o main.exe -I "C:\MinGW\include\SDL2" -lmingw32 -lSDL2main -lSDL2
*/

#include <SDL.h>
#include "include/objetos.hpp"
#include "include/cena.hpp"
#include "include/matriz.hpp"
#include "include/util.hpp"

double raycast (Lista<Objeto> &cena, Raio raio, Objeto* &atingido)
{
	// Vai guardar o menor valor de t 
	double menor_t = -1;

	// Percorrendo os objetos da cena
	for (auto obj : cena)
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
	// Ponto de visao da cena, origem dos raios
	Ponto origem = { 0, 0, 0 };

	// Informacoes da janela ***************************************************
	double wJanela = 60; // Largura da janela em CM
	double hJanela = 60; // Altura da janela em CM
	double dJanela = 30; // Distancia da janela em CM
	Janela janela (wJanela, hJanela, dJanela); // Objeto da janela

	// Informacoes do Canvas ***************************************************
	int nCol = 500; // Numero de colunas da grade do canvas
	int nLin = 500; // Numero de linhas da grade do canvas

	// Delta X e Y dos quadrados da grade do canvas ****************************
	double Dx = janela.getWidth() / nCol;
	double Dy = janela.getHeight() / nLin;

	// Funcao que define os vertices, arestas e faces da malha
	Cena::definirMalha();
	// Funcao que define a lista de objetos da cena
	Cena::definirCena();
	// Funcao que define a lista de fontes da cena
	Cena::definirFontes();

	// Informacoes da Camera ***************************************************
	// Visão de frente
	Ponto eye = { 0, 0, 0 };
	Ponto at = { 0, 0, -1 };
	Ponto up = { 0, 1, 0 };
	// Visão de cima
	// Ponto eye = { 0, 100, -80 };
	// Ponto at = { 0, 0, -80 };
	// Ponto up = { 0, 100, -100 };
	// Visão de lado direito
	// Ponto eye = { 100, 0, -100 };
	// Ponto at = { 0, 0, -100 };
	// Ponto up = { 100, 1, -100 };
	// Visão de lado esquerdo
	// Ponto eye = { -100, 0, -80 };
	// Ponto at = { 0, 0, -80 };
	// Ponto up = { -100, 1, -80 };
	// Objeto da Camera 
	Camera camera (eye, at, up);
	// Realizando transformacao de coordenadas de mundo em de camera
	camera.toCamera(Cena::cena, Cena::fontes);

	// Matriz de cores *********************************************************
	Cor** cores = new Cor*[nLin];
	// Inicializando cada linha da matriz
	for (int i = 0; i < nLin; ++i) { cores[i] = new Cor[nCol]; }

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
		double y = (janela.getHeight() / 2) - (Dy / 2) - (lin * Dy);

		// Percorre as colunas da grade do canvas
		for (int col = 0; col < nCol; ++col) 
		{
			// Coordenada X do centro do quadriculo no frame
			double x = -(janela.getWidth() / 2) + (Dx / 2) + (col * Dx);

			// Gerando raio lancado pela janela
			Ponto pontoJanela { x, y, -janela.getDistance() };
			Raio raio = Raio (origem, pontoJanela);

			// Armazena o objeto intersectado mais proximo 
			Objeto* atingido = nullptr; 
			// Armazena o valor de t que intersecta o objeto mais proximo
			double t_int = raycast(Cena::cena, raio, atingido);

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
					if (fonte->sombra(p_int, Cena::cena, raycast)) 
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
			} else { 
				cores[lin][col] = { 0, 0, 0 };
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
		"Computação Gráfica",             // Título da Janela
		SDL_WINDOWPOS_UNDEFINED,        // Posição inicial X
		SDL_WINDOWPOS_UNDEFINED,        // Posição inicial Y
		nCol,                           // Largura da janela em pixels
		nLin,                           // Altura da janela em pixels
		SDL_WINDOW_SHOWN                // Flags
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
			if (event.type == SDL_QUIT) {
				isRunning = false;
			}
		}
		// Limpando o renderer
		SDL_RenderClear(renderer);
		
		for (int y = 0; y < nLin; ++y) {
			for (int x = 0; x < nCol; ++x) {
				// Designando a cor que vai pintar
				SDL_SetRenderDrawColor(
					renderer, 
					cores[y][x].r, 
					cores[y][x].g, 
					cores[y][x].b, 
					SDL_ALPHA_OPAQUE
				);
				// Pintando o pixel
				SDL_RenderDrawPoint(renderer, x, y);
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