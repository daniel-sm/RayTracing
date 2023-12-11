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
#include "include/info_transform.hpp"
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
	// retornando o menor valor de t encontrado
    return menor_t;
}

void RayCasting (int linhas, int colunas, Cor** colors, Objeto*** hitted) 
{
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

			// Projecao (1 - perspectiva), (2 - ortografica), (3 - obliqua) 
			int projecao = Cena::info.success ? Cena::info.projecao : 1;
			Vetor dir_projecao = Cena::info.success ? Cena::info.dir_projecao : Vetor{ 0, 0, 0 };

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
				raio = Raio (pontoJanela, dir_projecao);
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
					bool sombra = fonte->sombra(p_int, Cena::cenario, CastRay);
					// Checando se o ponto esta na sombra
					if (sombra) {
						// Armazena a intensidade no ponto para a fonte atual
						Vetor atual = fonte->iluminacao(
							atingido->getNormal(p_int),
							p_int,
							raio.getDirecao(),
							atingido->material
						);
						// Soma a intensidade de cada fonte
						I = I + atual;
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
			colors[l][c].r = (int) (colors[l][c].r / maiorCor);
			colors[l][c].g = (int) (colors[l][c].g / maiorCor);
			colors[l][c].b = (int) (colors[l][c].b / maiorCor);
		}
	}
}

int main(int argc, char** argv) 
{
	// Funcao que define a lista de objetos da cena
	Cena::definirObjetos();
	// Funcao que define a lista de fontes da cena
	Cena::definirFontes();

	// Lendo e realizando transformacoes do arquivo
	InfoTransform transform ("info/transform.txt");

	// Informacoes do Canvas ***************************************************
	int linhas = 495; // 700; // Numero de linhas da grade do canvas
	int colunas = 880; // 1244; // Numero de colunas da grade do canvas

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
	RayCasting(linhas, colunas, colors, hitted);

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

	// Ponteiro do objeto que clicou
	Objeto* clicked = nullptr;

	// Controle de luz
	bool spot_on = true, pontual_on = true, dir_on = true;

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
				std::cout << "Objeto: " << clicked << "\n";
				break;

			case SDL_KEYDOWN:
				if (clicked != nullptr)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_DELETE:
						std::cout << "Removendo objeto: " << clicked << "...";
						Cena::cenario.remove(clicked);
						std::cout << " OK!\n";
						break; 
					case SDLK_SPACE:
						std::cout << "Realizando RayCasting...";
						clicked = nullptr;
						RayCasting(linhas, colunas, colors, hitted);
						std::cout << " OK!\n";
						break;
					default:
						break;
					}
				} else 
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_s:
						if (spot_on) {
							std::cout << "Desligando Spot...";
							Cena::fontes.remove(&(Cena::spot));
							spot_on = false;
							std::cout << " OK!\n";
						}
						else {
							std::cout << "Ligando Spot...";
							Cena::fontes.add(&(Cena::spot));
							spot_on = true;
							std::cout << " OK!\n"; 
						}
						break;
					case SDLK_d:
						if (dir_on) {
							std::cout << "Desligando Direcional...";
							Cena::fontes.remove(&(Cena::direcional)); 
							dir_on = false;
							std::cout << " OK!\n";
						}
						else {
							std::cout << "Ligando Direcional...";
							Cena::fontes.add(&(Cena::direcional)); 
							dir_on = true;
							std::cout << " OK!\n";
						}
						
						break;
					case SDLK_p:
						if (pontual_on) {
							std::cout << "Desligando Pontual...";
							Cena::fontes.remove(&(Cena::pontual)); 
							pontual_on = false;
							std::cout << " OK!\n";
						}
						else {
							std::cout << "Ligando Pontual...";
							Cena::fontes.add(&(Cena::pontual)); 
							pontual_on = true;
							std::cout << " OK!\n";
						}
						
						break;
					case SDLK_SPACE:
						std::cout << "Realizando RayCasting...";
						clicked = nullptr;
						RayCasting(linhas, colunas, colors, hitted);
						std::cout << " OK!\n";
						break;
					default:
						break;
					}
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