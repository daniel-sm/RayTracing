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

double raycast (Lista<Objeto> &cena, Raio raio, Objeto* &atingido)
{
	// Vai guardar o menor valor de t 
	double t_int = -1;

	// Percorrendo os objetos da cena
	for (auto obj : cena)
	{
        // Guardando o retorno da intersecao para comparar com 't_int'
        double t = obj->intersecao(raio);

        // Computando o menor valor de t_int
        if (t > 0) 
        {
            if (t_int > 0) 
            { 
                if (t < t_int) 
                { 
                    t_int = t; 
                    atingido = obj; 
                }
            } 
            else { t_int = t; atingido = obj; }
        }
    }
    return t_int;
}

int main(int argc, char** argv) 
{
	// Ponto de visao da cena, origem dos raios
	Ponto eye { 0, 0, 0 };

	// Informacoes da janela ***************************************************
	double wJanela = 60; // Largura da janela em CM
	double hJanela = 60; // Altura da janela em CM
	double dJanela = 30; // Distancia da janela em CM
	Janela janela (wJanela, hJanela, dJanela); // Objeto da janela

	// Informacoes do Canvas ***************************************************
	int nCol = 500; // Numero de colunas da grade do canvas
	int nLin = 500; // Numero de linhas da grade do canvas

	// Informacoes da Esfera ***************************************************
	double raioEsfera = 40; // Raio da esfera em CM
	Ponto centroEsfera { 0, 0, -100 }; // Centro da esfera
	// Propriedades de reflectividade da esfera
	Vetor kaEsfera { 0.7, 0.2, 0.2 }; // Propr. ambiente do material da esfera 
	Vetor kdEsfera = kaEsfera; // Propr. difusa do material da esfera 
	Vetor keEsfera = kaEsfera; // Propr. especular do material da esfera 
	double brilhoEsfera = 10; // Fator de brilho especular
	// Material da esfera
	Material materialEsfera { kaEsfera, kdEsfera, keEsfera, brilhoEsfera };
	// Objeto da Esfera
	Esfera esfera (centroEsfera, raioEsfera, materialEsfera);

	// Informacoes do Plano do Chao ********************************************
	Ponto pontoChao { 0, -raioEsfera, 0 }; // Ponto presente no plano do chao
	Vetor normalChao { 0, 1, 0 }; // Vetor normal ao plano do chao
	// Propriedades de reflectividade do plano do chao
	Vetor kaChao { 0.2, 0.7, 0.2 }; // Propr. ambiente do material do chao 
	Vetor kdChao = kaChao; // Propr. difusa do material do chao 
	Vetor keChao { 0.0, 0.0, 0.0 }; // Propr. difusa do material do chao 
	double brilhoChao = 1; // Fator de brilho espescular
	// Material do plano do chao
	Material materialChao { kaChao, kdChao, keChao, brilhoChao };
	// Objeto do Plano do Chao
	Plano chao (pontoChao, normalChao, materialChao);

	// Informacoes do Plano do Fundo *******************************************
	Ponto pontoFundo { 0, 0, -200 }; // Ponto presente no plano do fundo
	Vetor normalFundo { 0, 0, 1 }; // Vetor normal ao plano do fundo
	// Propriedades de reflectividade do plano de fundo
	Vetor kaFundo { 0.3, 0.3, 0.7 }; // Propr. ambiente do material do fundo 
	Vetor kdFundo = kaFundo; // Propr. difusa do material do fundo 
	Vetor keFundo { 0.0, 0.0, 0.0 }; // Propr. difusa do material do fundo 
	double brilhoFundo = 1; // Fator de brilho espescular
	// Material do plano de fundo
	Material materialFundo { kaFundo, kdFundo, keFundo, brilhoFundo };
	// Objeto do Plano do Fundo
	Plano fundo (pontoFundo, normalFundo, materialFundo);
	
	// Informacoes da Fonte Pontual ********************************************
	Vetor intensePontual { 0.7, 0.7, 0.7 };
	Ponto posicaoPontual { 0, 60, -30 };
	// Objeto da Fonte Pontual de luz
	Pontual pontual (intensePontual, posicaoPontual);

    // Informacoes da luz Ambiente *********************************************
    Vetor luzAmbiente { 0.3, 0.3, 0.3 };

	// Lista de Objetos da Cena ************************************************
	Lista<Objeto> cena;
	// Adicionando os objetos na cena
	cena.add(&esfera);
	cena.add(&chao);
	cena.add(&fundo);

	// Lista de Fontes de luz **************************************************
	Lista<Pontual> fontes;
	// Adicionando as fontes na lista
	fontes.add(&pontual);

	// Matriz de cores *********************************************************
	Cor** cores = new Cor*[nLin];
	// Inicializando cada linha da matriz
	for (int i = 0; i < nLin; ++i) { cores[i] = new Cor[nCol]; }

	// Maior intensidade de cor ************************************************
	// Usada para reprocessar as cores com base na maior
	double maiorCor = 1.0; 

	// Delta X e Y dos quadrados da grade do canvas ****************************
	double Dx = janela.getWidth()/nCol;
	double Dy = janela.getHeight()/nLin;

	// Loop de Ray Casting *****************************************************
	// Lancando raios para cada quadrado na grade do canvas
	//
	// Percorre as linhas da grade do canvas
	for (int lin = 0; lin < nLin; ++lin) 
	{
		// Coordenada Y do centro do quadriculo no frame
		double y = (janela.getHeight()/2) - Dy/2 - lin*Dy;

		// Percorre as colunas da grade do canvas
		for (int col = 0; col < nCol; ++col) 
		{
			// Coordenada X do centro do quadriculo no frame
			double x = -(janela.getWidth()/2) + (Dx/2) + (col*Dx);

			// Gerando raio lancado pela janela
			Raio raio = Raio (eye, { x, y, -janela.getDistance() });

			// Armazena o objeto intersectado mais proximo 
			Objeto* atingido = nullptr; 
			// Armazena o valor de t que intersecta o objeto mais proximo
			double t_int = raycast(cena, raio, atingido);

			// Verificando de atingiu algum objeto
			if (t_int > 0) 
			{
				// Obtendo o ponto de intersecao a partir do t encontrado
                Ponto p_int = raio.pontoIntersecao(t_int);

				// Vetor de intensidade da luz no ponto intersectado
				Vetor I { 0.0, 0.0, 0.0 };
				// Adicionando a luz ambiente
				I = I + (luzAmbiente * atingido->material.ka);

				// Percorrendo as fontes de luz da cena
				for (auto fonte : fontes)
				{
					// Obtem a posicao da fonte pontual
					Ponto posFonte = fonte->getPosicao();
					// Gerando um raio da fonte para o ponto intersectado
					Raio raioSombra (posFonte, p_int);
					// Ponteiro para o objeto que foi acertado pelo raio 
					Objeto* acertado = nullptr;

					// Calculando a distancia da fonte ao objeto acertado
					double t_sombra = raycast(cena, raioSombra, acertado);
					// Calculando a distancia da fonte ao ponto intersectado
					double distanciaFonte = norma(p_int - posFonte);
					
					// Compara as distancias
					if (t_sombra + 0.01 >= distanciaFonte) 
					{
						Vetor normal = atingido->obterNormal(p_int);

						// Armazena a intensidade no ponto para a fonte atual
						Vetor aux = fonte->calcIntensidade(
							normal,
							p_int,
							raio.direcao(),
							atingido->material
						);

						// Soma a intensidade de cada fonte
						I = I + aux;
					}
				}

				// Computando a maior intensidade de cor
				if (I.a > maiorCor) maiorCor = I.a;
				if (I.b > maiorCor) maiorCor = I.b;
				if (I.c > maiorCor) maiorCor = I.c;

				Cor cor { 255, 255, 255 };

				// Arredondar para valor inteiro
				cor.r = round(cor.r * I.a);
				cor.g = round(cor.g * I.b);
				cor.b = round(cor.b * I.c);

				cores[lin][col] = cor;
			} else { 
				cores[lin][col] = { 0, 0, 0 };
			}
		}
	}

	// Reprocessar as cores com base na maior intensidade de cor
	for (int l = 0; l < nLin; ++l) {
		for (int c = 0; c < nCol; ++c) {
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