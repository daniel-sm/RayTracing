/*
Instalar sdl2 no Ubuntu:
sudo apt-get install libsdl2-dev

Compilar sdl2 no Ubuntu:
g++ main.cpp -o main.out `sdl2-config --cflags --libs`

Compilar sdl2 no Windows
g++ main.cpp -o main.exe -I "C:\MinGW\include\SDL2" -lmingw32 -lSDL2main -lSDL2
*/

#include <SDL.h>

struct Cor { int r, g, b; };

struct Vetor {
	double a, b, c;

	Vetor operator*(double k) {
		return { a * k, b * k, c * k };
	}
	
	Vetor operator/(double k) {
		return { a / k, b / k, c / k };
	}

	Vetor operator+(Vetor V) {
		return { (a + V.a), (b + V.b), (c + V.c) };
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

double ProdutoEscalar (Vetor v, Vetor u) {
	return (v.a * u.a) + (v.b * u.b) + (v.c * u.c);
}

class Raio {
	public:
 
	Ponto inicial;
	Vetor direcao;

	Raio(Ponto pin, Ponto pr) : inicial{pin}, direcao{pr - pin} {
		inicial = pin;
		Vetor v = pr - pin;
		direcao = v / SDL_sqrt(ProdutoEscalar(v, v));
	}
	
	Ponto P (double t) { return { inicial + (direcao * t) }; }
};

class Esfera {
	Ponto centro; 
	double raio;

	public:
	Esfera(Ponto c, double r) : centro{c}, raio{r} {}
};

int main(int argc, char* argv[]) {

	Ponto olho { 0, 0, 0 };

	double wJanela = 10; // Largura da janela
	double hJanela = 10; // Altura da janela
	double dJanela = 1; // Distancia da janela
	// Ponto cJanela { 0, 0, -dJanela }; // Coordenada do centro da janela

	double rEsfera = 5; // Raio da esfera
	Ponto cEsfera { 0, 0, -(dJanela + rEsfera) }; // Coord. do centro da esfera

	Cor esfColor { 255, 0, 0 }; // Cor da esfera
	Cor bgColor { 100, 100, 100 }; // Cor de fundo

	int nCol = 700; // Numero de colunas da grade
	int nLin = 700; // Numero de linhas da grade

	Cor** matrizCores = new Cor*[nLin]; // Matriz de cores

	for (int i = 0; i < nLin; i++) { // Inicializando linhas da matriz
		matrizCores[i] = new Cor[nCol];
	}

	// Delta X e Y dos quadriculos da grade
	double Dx = wJanela/nCol;
	double Dy = hJanela/nLin;

	for (int l = 0; l < nLin; l++) {
		double y = hJanela/2 - Dy/2 - l*Dy;

		for (int c = 0; c < nCol; c++) {
			double x = -(wJanela/2) + (Dx/2) + (c*Dx);

			Raio R = Raio (olho, { x, y, -dJanela });
			
			Vetor W = R.inicial - cEsfera;

			double B = ProdutoEscalar(W, R.direcao);
			double C = ProdutoEscalar(W, W) - rEsfera*rEsfera;

			double delta = (B * B) - C;

			if (delta < 0) matrizCores[l][c] = bgColor;
			else           matrizCores[l][c] = esfColor;
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
					matrizCores[x][y].r, 
					matrizCores[x][y].g, 
					matrizCores[x][y].b, 
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

	return 0;
}