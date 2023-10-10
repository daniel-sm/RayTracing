/*
Instalar sdl2 no Ubuntu:
sudo apt-get install libsdl2-dev

Compilar sdl2 no Ubuntu:
g++ main.cpp -o main.out `sdl2-config --cflags --libs`

Compilar sdl2 no Windows
g++ main.cpp -o main.exe -I "C:\MinGW\include\SDL2" -lmingw32 -lSDL2main -lSDL2
*/

#include <SDL.h>
#include <cmath>

struct Cor { int r, g, b; };

struct Vetor {
	double a, b, c;

	Vetor operator*(double k) {
		return { (a * k), (b * k), (c * k) };
	}
	
	Vetor operator*(Vetor u) {
		return { (a * u.a), (b * u.b), (c * u.c) };
	}
	
	Vetor operator/(double k) {
		return { (a / k), (b / k), (c / k) };
	}

	Vetor operator+(Vetor V) {
		return { (a + V.a), (b + V.b), (c + V.c) };
	}

	Vetor operator-(Vetor V) {
		return { (a - V.a), (b - V.b), (c - V.c) };
	}
};

struct Ponto {
	double x, y, z;

	Ponto operator+ (Vetor V) {
		return { (x + V.a), (y + V.b), (z + V.c) };
	}

	Vetor operator- (Ponto P) {
		return { (x - P.x), (y - P.y), (z - P.z) };
	}
};

double escalar (Vetor v, Vetor u) {
	return (v.a * u.a) + (v.b * u.b) + (v.c * u.c);
}

double norma (Vetor v) {
	return sqrt(escalar(v, v));
}

Vetor unitario (Vetor v) {
	return (v / norma(v));
}

class Raio {
	Ponto ini; // origem
	Vetor dir; // unitario

	public:
 
	Raio(Ponto pin, Ponto pr) {
		ini = pin;
		dir = unitario(pr - pin);
	}

	Vetor direcao() { return dir; }
	Ponto origem() { return ini; }

	// P: "Pint", ponto de intersecao com parametro "t" 
	Ponto P (double t) { return { ini + (dir * t) }; }
};

class Esfera {
	Ponto centro; 
	double raio;

	public:
	Esfera(Ponto c, double r) : centro{c}, raio{r} {}

	double intersecao(Raio r) {
		Vetor w = r.origem() - centro;

		double b = escalar(w, r.direcao());
		double c = escalar(w, w) - (raio*raio);

		double delta = (b * b) - c;

		if (delta > 0) {
			if (c < 0) {
				return -b + sqrt(delta);
			} else {
				return -b - sqrt(delta);
			}
		}
		return -1; // se nada ocorrer = nao intersecta
	}
};

int main(int argc, char* argv[]) {
// Ponto de visao da cena, origem dos raios
	Ponto olho { 0, 0, 0 };

	// Informacoes da luz 
	Ponto pFonte { 0, 5, 0 };
	Vetor iFonte { 0.7, 0.7, 0.7 };

	// Fator do material da esfera sobre uma luz
	Vetor kEsfera { 1.0, 0.0, 0.0 }; 
	double mEsfera = 10; // fator espescular

	double wJanela = 2; // Largura da janela
	double hJanela = 2; // Altura da janela
	double dJanela = 1; // Distancia da janela
	
	// Tamanho do raio da esfera 
	double rEsfera = 1;

	// Coordenadas do centro da esfera
	Ponto cEsfera { 0, 0, -(dJanela + rEsfera) };

	Cor esfColor { 255, 0, 0 }; // Cor da esfera
	Cor bgColor { 100, 100, 100 }; // Cor de fundo

	int nCol = 600; // Numero de colunas da grade
	int nLin = 600; // Numero de linhas da grade

	// Matriz de cores
	Cor** mCores = new Cor*[nLin];

	// Inicializando linhas da matriz
	for (int i = 0; i < nLin; i++) {
		mCores[i] = new Cor[nCol];
	}

	// Delta X e Y dos quadriculos da grade
	double Dx = wJanela/nCol;
	double Dy = hJanela/nLin;

	for (int lin = 0; lin < nLin; lin++) {
		// Coordenada Y do centro do quadriculo no frame
		double y = hJanela/2 - Dy/2 - lin*Dy;

		for (int col = 0; col < nCol; col++) {
			// Coordenada X do centro do quadriculo no frame
			double x = -(wJanela/2) + (Dx/2) + (col*Dx);

			Raio R = Raio (olho, { x, y, -dJanela });
			Esfera E = Esfera (cEsfera, rEsfera);

			double t = E.intersecao(R);

			if (t > 0) {
				Ponto pInt = R.P(t);

				Vetor l = unitario(pFonte - pInt);
				Vetor n = (pInt - cEsfera) / rEsfera;
				Vetor v = R.direcao() * -1;
				Vetor r = (n * 2 * escalar(l, n)) - l;

				Vetor luz = (kEsfera * iFonte);

				double fatorDif = escalar(l, n);

				Vetor Id { 0.0, 0.0, 0.0 };
				if (fatorDif >= 0) Id = luz * fatorDif;

				double fatorEsp = escalar(v, r);

				Vetor Ie { 0.0, 0.0, 0.0 };
				if (fatorEsp >= 0) {
					fatorEsp = pow(fatorEsp, mEsfera);
					Ie = luz * fatorEsp;
				}

				Vetor I = Id + Ie;

				Cor cor { 255, 255, 255 };

				cor.r = round(cor.r * I.a);
				cor.g = round(cor.g * I.b);
				cor.b = round(cor.b * I.c);

				if (cor.r > 255) cor.r = 255;
				if (cor.g > 255) cor.g = 255;
				if (cor.b > 255) cor.b = 255;

				mCores[lin][col] = cor;
			}
			else { 
				mCores[lin][col] = bgColor;
}
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
		
		for (int i = 0; i < nLin; i++) {
			for (int j = 0; j < nCol; j++) {
				// Designando a cor que vai pintar
				SDL_SetRenderDrawColor(
					renderer, 
					mCores[i][j].r, 
					mCores[i][j].g, 
					mCores[i][j].b, 
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

	// Desalocando a matriz de cores
	for (int i = 0; i < nLin; i++) {
		delete mCores[i];
	}
	delete[] mCores;

	return 0;
}