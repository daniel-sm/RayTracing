#ifndef CENA_HPP
#define CENA_HPP

#include "objetos.hpp"
#include "complexos.hpp"
#include "transformacoes.hpp"

namespace Cena
{
	// Materiais dos objetos 
	//
	// Material do Plano do Chao ***********************************************
	// Propriedades de reflectividade do plano do chao
	Vetor kaChao { 0.2, 0.7, 0.2 }; // Prop ambiente do material do chao 
	Vetor kdChao = kaChao; // Prop difusa do material do chao 
	Vetor keChao { 0.0, 0.0, 0.0 }; // Prop especular do material do chao 
	double brilhoChao = 1; // Fator de brilho espescular
	// Material do plano do chao
	Material materialChao { kaChao, kdChao, keChao, brilhoChao };

	// Material do Plano de Fundo **********************************************
	// Propriedades de reflectividade do plano de fundo
	Vetor kaFundo { 0.3, 0.3, 0.7 }; // Prop ambiente do material do fundo 
	Vetor kdFundo = kaFundo; // Prop difusa do material do fundo 
	Vetor keFundo { 0.0, 0.0, 0.0 }; // Prop especular do material do fundo 
	double brilhoFundo = 1; // Fator de brilho especular
	// Material do plano de fundo
	Material materialFundo { kaFundo, kdFundo, keFundo, brilhoFundo };

	// Material da Casa ********************************************************
	// Propriedades de reflectividade da Casa
	Vetor kaCasa = { 1.0, 1.0, 1.0 }; // Prop ambiente do material da Casa
	Vetor kdCasa = kaCasa; // Prop difusa do material da Casa
	Vetor keCasa = kaCasa; // Prop especular do material da Casa
	double brilhoCasa = 10; // Fator de brilho especular
	// Material da Casa
	Material materialCasa { kaCasa, kdCasa, keCasa, brilhoCasa };

	// Material do Telhado *****************************************************
	// Propriedades de reflectividade do Telhado
	Vetor kaTelhado = { 0.7, 0.4, 0.3 }; // Prop ambiente do material do Telhado
	Vetor kdTelhado = kaTelhado; // Prop difusa do material do Telhado
	Vetor keTelhado = kaTelhado; // Prop especular do material do Telhado
	double brilhoTelhado = 5; // Fator de brilho especular
	// Material do Telhado
	Material materialTelhado { kaTelhado, kdTelhado, keTelhado, brilhoTelhado };

	// Material do Poste *******************************************************
	// Propriedades de reflectividade do Poste
	Vetor kaPoste = { 0.4, 0.4, 0.4 }; // Prop ambiente do material do Poste
	Vetor kdPoste = kaPoste; // Prop difusa do material do Poste
	Vetor kePoste = { 0.5, 0.5, 0.5 }; // Prop especular do material do Poste
	double brilhoPoste = 5; // Fator de brilho especular
	// Material do Poste
	Material materialPoste { kaPoste, kdPoste, kePoste, brilhoPoste };

	// Material da Arvore ******************************************************
	// Propriedades de reflectividade da Arvore
	Vetor kaArvore = { 0.0, 1.0, 0.0 }; // Prop ambiente do material da Arvore
	Vetor kdArvore = kaArvore; // Prop difusa do material da Arvore
	Vetor keArvore = kaArvore; // Prop especular do material da Arvore
	double brilhoArvore = 5; // Fator de brilho especular
	// Material da Arvore
	Material materialArvore { kaArvore, kdArvore, keArvore, brilhoArvore };

	// Material do Tronco ******************************************************
	// Propriedades de reflectividade do Tronco
	Vetor kaTronco = { 0.4, 0.3, 0.1 }; // Prop ambiente do material do Tronco
	Vetor kdTronco = kaTronco; // Prop difusa do material do Tronco
	Vetor keTronco = kaTronco; // Prop especular do material do Tronco
	double brilhoTronco = 5; // Fator de brilho especular
	// Material do Tronco
	Material materialTronco { kaTronco, kdTronco, keTronco, brilhoTronco };

	// *************************************************************************
	// Objetos da Cena *********************************************************
	//
	// Objetos em coordenadas de mundo onde todos objetos estao 
	// no primeiro octante do sistema de coordenadas. O plano do 
	// chao é o plano XZ (y = 0) e os planos XY (z = 0) e YZ (x = 0) 
	// sao delimitantes de fundo para o cenario
	//
	// Informacoes do Plano do Chao ********************************************
	Ponto pontoChao { 0, 0, 0 }; // Ponto presente no plano do chao
	Vetor normalChao { 0, 1, 0 }; // Vetor normal ao plano do chao
	// Objeto do Plano do Chao
	Plano chao (pontoChao, normalChao, materialChao);

	// Informacoes do Plano do Fundo Esquerdo **********************************
	Ponto pontoFundoEsq { 0, 0, 0 }; // Ponto do plano do Fundo Esquerdo
	Vetor normalFundoEsq { 1, 0, 0 }; // Vetor normal ao plano do Fundo Esquerdo
	// Objeto do Plano do Fundo Esquerdo
	Plano fundoesq (pontoFundoEsq, normalFundoEsq, materialFundo);

	// Informacoes do Plano do Fundo Direito ***********************************
	Ponto pontoFundoDir { 0, 0, 0 }; // Ponto presente no plano do fundo Direito
	Vetor normalFundoDir { 0, 0, 1 }; // Vetor normal ao plano do fundo Direito
	// Objeto do Plano do Fundo Direito
	Plano fundodir (pontoFundoDir, normalFundoDir, materialFundo);

	// Objeto Complexo da Casa *************************************************
	// Composto de um cubo e uma piramide
	Casa casa (materialCasa, materialTelhado);
	// Objeto Complexo da Arvore ***********************************************
	// Composto de uma esfera e um cone
	Arvore arvore1 (materialArvore, materialTronco);
	Arvore arvore2 (materialArvore, materialTronco);
	// Objeto Complexo do Poste ************************************************
	// Composto de dois cilindro
	Poste poste (materialPoste);

	// Textura *****************************************************************
	SDL_Surface *imagem = SDL_LoadBMP("door_1348x1500.bmp"); // Carrega a imagem 
	Ponto esq_inf { 0, 0, 0 }; // Define o ponto inferior esquerdo 
	Ponto dir_inf { 1, 0, 0 }; // Define o ponto inferior direito 
	Ponto dir_sup { 1, 1, 0 }; // Define o ponto superior direito 
	Ponto esq_sup { 0, 1, 0 }; // Define o ponto superior esquerdo 
	Textura textura (esq_inf, dir_inf, dir_sup, esq_sup, imagem);

	// *************************************************************************
	// Fontes de luz ***********************************************************
	//
	// Informacoes da Fonte Pontual ********************************************
	Vetor intensePontual = { 1.0, 1.0, 1.0 }; // Intensidade da fonte pontual
	Ponto posicaoPontual = { 1000, 500, 1000 }; // Posicao da fonte pontual
	// Objeto da fonte de luz Pontual
	Pontual pontual (intensePontual, posicaoPontual);

	// Informacoes da Fonte Spot ***********************************************
	Vetor intenseSpot = { 0.5, 0.5, 0.5 }; 
	Ponto posicaoSpot = { 0, 0, 0 };
	Vetor direcaoSpot = { 0, -1, 0 };
	double anguloSpot = (3 * PI) / 8.0;
	// Objeto da fonte de luz Spot
	Spot spot (intenseSpot, posicaoSpot, direcaoSpot, anguloSpot);

	// Informacoes da Fonte Direcional *****************************************
	Vetor intenseDirecional = { 0.5, 0.5, 0.5 };
	Vetor direcaoDirecional = { -1, -1, -1 };
	// Objeto da fonte de luz Direcional
	Direcional direcional (intenseDirecional, direcaoDirecional);

    // Informacoes da luz Ambiente *********************************************
    Vetor luzAmbiente { 0.3, 0.3, 0.3 };

	// Informacoes da Janela ***************************************************
	double xminJanela = -72; // Valor da parte de cima da janela em cm
	double xmaxJanela = 72; // Valor da parte de baixo da janela em cm
	double yminJanela = -40.5; // Valor do lado esquerdo da janela em cm
	double ymaxJanela = 40.5; // Valor do lado direito da janela em cm
	double dJanela = 150; // Distancia da janela em cm
	// Objeto da Janela
	Janela janela (xminJanela, xmaxJanela, yminJanela, ymaxJanela, dJanela); 

	// Informacoes da Camera ***************************************************
	// Vista em Diagonal
	Ponto eye = { 2600, 1000, 6000 };
	Ponto at = { 2600, 0, 0 };
	Ponto up = { 2600, 500, 0 }; 
	// Objeto da Camera 
	Camera camera (eye, at, up);

	// Lista de Objetos da Cena ************************************************
	Lista<Objeto> cenario;

    // Definindo a lista de objetos da cena
    void definirObjetos() 
    {
		// Realizando transformacoes 
		// Ajustando objeto da casa
		Transformacao::escala(&casa, { 500, 300, 400 });
		Transformacao::translacao(&casa, { 2600, 0, 2200 });
		// Ajustando objeto da arvore
		Transformacao::escala(&arvore1, { 2, 1, 1 });
		Transformacao::translacao(&arvore1, { 2400, 0, 2400 });
		Transformacao::translacao(&arvore2, { 2400, 0, 2400 });
		Transformacao::espelhoArbitrario(&arvore2,  { 2850, 0, 2200 }, { -1, 0, 0 });
		// Ajustando objeto do poste
		Transformacao::translacao(&poste, { 2400, 0, 2800 });
		// Ajustando objeto da textura
		if (imagem) {
			Transformacao::escala(&textura, { 200, 222.551928701, 0 });
			Transformacao::translacao(&textura, { 2750, 0, 2601 });
			cenario.add(&textura);
		} else SDL_Log("Erro ao abrir imagem da textura! SDL_Error: %s", SDL_GetError());

        // Adicionando os objetos na cena 
		cenario.add(&arvore1);
		cenario.add(&arvore2);
		cenario.add(&poste);
		cenario.add(&casa); 
        cenario.add(&chao);
        cenario.add(&fundoesq);
        cenario.add(&fundodir);
    }

	// Lista de Fontes de luz **************************************************
	Lista<Fonte> fontes;

    // Definindo a lista de fontes da cena
    void definirFontes() 
    {
		// Realizando transformacoes 
		// Ajustando objeto da luz do poste
		// Posiciona a fonte spot na ponta do poste
		Transformacao::translacao(&spot, { 2500, 500-13, 2800 });
		// Transformacao::rotacaoZ(&spot, PI / 4);

        // Adicionando as fontes na lista 
        fontes.add(&spot);
        fontes.add(&pontual);
        fontes.add(&direcional); 
    }
} // namespace Cena

#endif