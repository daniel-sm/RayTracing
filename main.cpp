/*
Instalar sdl2 no Ubuntu:
sudo apt-get install libsdl2-dev

Compilar sdl2 no Ubuntu:
g++ main.cpp -o main.out `sdl2-config --cflags --libs`

Compilar sdl2 no Windows
g++ main.cpp -o main.exe -I "C:\MinGW\include\SDL2" -lmingw32 -lSDL2main -lSDL2
*/

#include <SDL.h>
#include <SDL_image.h>
#include "include/cena.hpp"
#include "include/objetos.hpp"
#include "include/intersecoes.hpp"
#include "include/transformacoes.hpp"
#include "include/basics.hpp"

double CastRay (Lista<Objeto> &cenario, Raio raio, Objeto* &atingido)
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

void RayCasting (
	int linhas, int colunas, 
	int projecao, Vetor dirProjecao, 
	Cor** colors, Objeto*** hitted
) {
	// Delta X e Y dos quadrados da grade do canvas ****************************
	double Dx = Cena::janela.getWidth() / colunas;
	double Dy = Cena::janela.getHeight() / linhas;

	// Maior intensidade de cor ************************************************
	// Usada para reprocessar as cores com base na maior
	double maiorCor = 1.0; 

	// Loop de Ray Casting *****************************************************
	// Lancando raios para cada quadrado na grade do canvas
	//
	// Percorre as linhas da grade do canvas
	for (int l = 0; l < linhas; ++l) 
	{
		// Coordenada Y do centro do quadriculo na Janela
		double y = Cena::janela.getYMax() - (Dy / 2) - (l * Dy);

		// Percorre as colunas da grade do canvas
		for (int c = 0; c < colunas; ++c) 
		{
			// Coordenada X do centro do quadriculo na Janela
			double x = Cena::janela.getXMin() + (Dx / 2) + (c * Dx);

			// Ponto do centro do quadrado da Janela
			Ponto pontoJanela { x, y, -Cena::janela.getDistance() };

			// Raio que vai ser lancado pela Janela
			Raio raio = Raio({ 0, 0, 0 }, pontoJanela); // Perspectiva por padrao

			// Gerando raio de acordo com a projecao
			switch (projecao)
			{
			case 1: // Caso seja persperctiva
				raio = Raio (Ponto{ 0, 0, 0 }, pontoJanela);
				break;
			case 2: // Caso seja ortografica
				raio = Raio (pontoJanela, Vetor{0, 0, -1});
				break;
			case 3: // Caso seja obliqua
				raio = Raio (pontoJanela, dirProjecao);
				break;
			default: 
				break;
			}

			// Armazena o objeto intersectado mais proximo 
			Objeto* atingido = nullptr; 
			// Armazena o valor de t que intersecta o objeto mais proximo
			double t_int = CastRay(Cena::cenario, raio, atingido);

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
					if (fonte->sombra(p_int, Cena::cenario, CastRay)) 
					{
						Vetor normal = atingido->getNormal(p_int);

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

				colors[l][c] = cor;
				hitted[l][c] = atingido;
			} else { 
				colors[l][c] = { 0, 0, 0 };
				hitted[l][c] = nullptr;
			}
		}
	}

	// Reprocessar as cores com base na maior intensidade de cor
	for (int l = 0; l < linhas; ++l) {
		for (int c = 0; c < colunas; ++c) {
			// Arredondando para valor inteiro 
			colors[l][c].r = round(colors[l][c].r / maiorCor);
			colors[l][c].g = round(colors[l][c].g / maiorCor);
			colors[l][c].b = round(colors[l][c].b / maiorCor);
		}
	}
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
	Vetor dirProjecao = { 0.2, 0, -1 };

	// Informacoes do Canvas ***************************************************
	int linhas = 495 /*700*/; // Numero de linhas da grade do canvas
	int colunas = 880 /*1244*/; // Numero de colunas da grade do canvas

	// Transformando de Mundo para Camera
	Cena::camera.toCamera(Cena::cenario, Cena::fontes);

	// Matriz de cores *********************************************************
	Cor** colors = new Cor*[linhas];
	// Inicializando cada linha da matriz
	for (int i = 0; i < linhas; ++i) { colors[i] = new Cor[colunas]; }

	// Matriz de objetos atingidos *********************************************
	// Array de arrays de ponteiro para Objeto
	Objeto*** hitted = new Objeto**[linhas];
	// Inicializando cada linha da matriz
	for (int i = 0; i < linhas; ++i) { hitted[i] = new Objeto*[colunas]; }

	// Realizando o RayCasting *************************************************
	// Chama funcao que percorre o canvas e lanca os raios pela janela
	RayCasting(linhas, colunas, projecao, dirProjecao, colors, hitted);

	// *************************************************************************
    // Utilizando SDL **********************************************************
	//
	// Inicializando a biblioteca SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("Não foi possível inicializar o SDL! SDL_Error: %s", SDL_GetError());
		return 1;
	}


	// Criando uma janela
	SDL_Window* window = SDL_CreateWindow(
		"Computação Gráfica",   // Título da Janela
		SDL_WINDOWPOS_CENTERED, // Posição inicial X
		SDL_WINDOWPOS_CENTERED, // Posição inicial Y
		colunas,                // Largura da janela em pixels
		linhas,                 // Altura da janela em pixels
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
	Objeto* clicked = nullptr;

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
				clicked = hitted[event.button.y][event.button.x];
				break;

			case SDL_KEYDOWN:
				if (clicked != nullptr)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_x:
						Cena::camera.toWorld(Cena::cenario, Cena::fontes);
						Transformacao::translacao(clicked, { 10, 0, 0 });
						Cena::camera.toCamera(Cena::cenario, Cena::fontes);
						break;
					case SDLK_y:
						Cena::camera.toWorld(Cena::cenario, Cena::fontes);
						Transformacao::translacao(clicked, { 0, 10, 0 });
						Cena::camera.toCamera(Cena::cenario, Cena::fontes);
						break;
					case SDLK_z:
						Cena::camera.toWorld(Cena::cenario, Cena::fontes);
						Transformacao::translacao(clicked, { 0, 0, 10 });
						Cena::camera.toCamera(Cena::cenario, Cena::fontes);
						break;
					default:
						break;
					}
					RayCasting(linhas, colunas, projecao, dirProjecao, colors, hitted);
				}
				break;

			default:
				break;
			}
		}
		// Limpando o renderer
		SDL_RenderClear(renderer);
		
		for (int i = 0; i < linhas; ++i) {
			for (int j = 0; j < colunas; ++j) {
				// Designando a cor que vai pintar
				SDL_SetRenderDrawColor(
					renderer, 
					colors[i][j].r, 
					colors[i][j].g, 
					colors[i][j].b, 
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
	for (int i = 0; i < linhas; ++i) { delete colors[i]; }
	delete[] colors; // Desaloca a matriz por completo

	return 0;
}