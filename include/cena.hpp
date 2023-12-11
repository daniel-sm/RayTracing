#ifndef CENA_HPP
#define CENA_HPP

#include "basics.hpp"
#include "objetos.hpp"
#include "complexos.hpp"
#include "transformacoes.hpp"
#include "info.hpp"
// #include <SDL_image.h>

namespace Cena
{
	// #########################################################################
	// Materiais dos objetos ###################################################
	//
	// Material do Plano do Chao ***********************************************
	// Propriedades de reflectividade do plano do chao
	Vetor kaChao { 0.2, 0.7, 0.2 }; // Prop ambiente do material do chao 
	Vetor kdChao = kaChao; // Prop difusa do material do chao 
	Vetor keChao { 0.0, 0.0, 0.0 }; // Prop especular do material do chao 
	double brilhoChao = 1; // Fator de brilho espescular
	// Material do plano do chao
	Material mtChao { kaChao, kdChao, keChao, brilhoChao };

/*
	// Material do Plano de Fundo **********************************************
	// Propriedades de reflectividade do plano de fundo
	Vetor kaFundo { 0.3, 0.3, 0.7 }; // Prop ambiente do material do fundo 
	Vetor kdFundo = kaFundo; // Prop difusa do material do fundo 
	Vetor keFundo { 0.0, 0.0, 0.0 }; // Prop especular do material do fundo 
	double brilhoFundo = 1; // Fator de brilho especular
	// Material do plano de fundo
	Material mtFundo { kaFundo, kdFundo, keFundo, brilhoFundo };
*/

	// Material da Casa ********************************************************
	// Propriedades de reflectividade da Casa
	Vetor kaCasa = { 1.0, 1.0, 1.0 }; // Prop ambiente do material da Casa
	Vetor kdCasa = kaCasa; // Prop difusa do material da Casa
	Vetor keCasa = kaCasa; // Prop especular do material da Casa
	double brilhoCasa = 10; // Fator de brilho especular
	// Material da Casa
	Material mtCasa { kaCasa, kdCasa, keCasa, brilhoCasa };

	// Material do Telhado *****************************************************
	// Propriedades de reflectividade do Telhado
	Vetor kaTelhado = { 0.7, 0.4, 0.3 }; // Prop ambiente do material do Telhado
	Vetor kdTelhado = kaTelhado; // Prop difusa do material do Telhado
	Vetor keTelhado = kaTelhado; // Prop especular do material do Telhado
	double brilhoTelhado = 10; // Fator de brilho especular
	// Material do Telhado
	Material mtTelhado { kaTelhado, kdTelhado, keTelhado, brilhoTelhado };

	// Material do Poste *******************************************************
	// Propriedades de reflectividade do Poste
	Vetor kaPoste = { 0.4, 0.4, 0.4 }; // Prop ambiente do material do Poste
	Vetor kdPoste = kaPoste; // Prop difusa do material do Poste
	Vetor kePoste = { 0.5, 0.5, 0.5 }; // Prop especular do material do Poste
	double brilhoPoste = 5; // Fator de brilho especular
	// Material do Poste
	Material mtPoste { kaPoste, kdPoste, kePoste, brilhoPoste };

	// Material da Folha *******************************************************
	// Propriedades de reflectividade da Folha
	Vetor kaFolha = { 0.0, 1.0, 0.0 }; // Prop ambiente do material da Folha
	Vetor kdFolha = kaFolha; // Prop difusa do material da Folha
	Vetor keFolha = kaFolha; // Prop especular do material da Folha
	double brilhoFolha = 5; // Fator de brilho especular
	// Material da Folha
	Material mtFolha { kaFolha, kdFolha, keFolha, brilhoFolha };

	// Material do Tronco ******************************************************
	// Propriedades de reflectividade do Tronco
	Vetor kaTronco = { 0.4, 0.3, 0.1 }; // Prop ambiente do material do Tronco
	Vetor kdTronco = kaTronco; // Prop difusa do material do Tronco
	Vetor keTronco = kaTronco; // Prop especular do material do Tronco
	double brilhoTronco = 5; // Fator de brilho especular
	// Material do Tronco
	Material mtTronco { kaTronco, kdTronco, keTronco, brilhoTronco };

	// Material da Lampada ******************************************************
	// Propriedades de reflectividade da Lampada
	Vetor kaLampada = { 1.0, 1.0, 1.0 }; // Prop ambiente do material da Lampada
	Vetor kdLampada = kaLampada; // Prop difusa do material da Lampada
	Vetor keLampada = kaLampada; // Prop especular do material da Lampada
	double brilhoLampada = 20; // Fator de brilho especular
	// Material da Lampada
	Material mtLampada { kaLampada, kdLampada, keLampada, brilhoLampada };

	// Material do Suporte ******************************************************
	// Propriedades de reflectividade do Suporte
	Vetor kaSuporte = { 0.1, 0.1, 0.1 }; // Prop ambiente do material do Suporte
	Vetor kdSuporte = kaSuporte; // Prop difusa do material do Suporte
	Vetor keSuporte = kaSuporte; // Prop especular do material do Suporte
	double brilhoSuporte = 15; // Fator de brilho especular
	// Material do Suporte
	Material mtSuporte { kaSuporte, kdSuporte, keSuporte, brilhoSuporte };

	// #########################################################################
	// Objetos da Cena #########################################################
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
	Plano chao (pontoChao, normalChao, mtChao);

/*
	// Informacoes do Plano do Fundo Esquerdo **********************************
	Ponto pontoFundoEsq { 0, 0, 0 }; // Ponto do plano do Fundo Esquerdo
	Vetor normalFundoEsq { 1, 0, 0 }; // Vetor normal ao plano do Fundo Esquerdo
	// Objeto do Plano do Fundo Esquerdo
	Plano fundoesq (pontoFundoEsq, normalFundoEsq, mtFundo);

	// Informacoes do Plano do Fundo Direito ***********************************
	Ponto pontoFundoDir { 0, 0, 0 }; // Ponto presente no plano do fundo Direito
	Vetor normalFundoDir { 0, 0, 1 }; // Vetor normal ao plano do fundo Direito
	// Objeto do Plano do Fundo Direito
	Plano fundodir (pontoFundoDir, normalFundoDir, mtFundo);
*/

	// Objeto Complexo da Casa *************************************************
	// Composto de um cubo e uma piramide
	Casa casa (mtCasa, mtTelhado);
	// Objeto Complexo da Arvore ***********************************************
	double alturaArv = 500;
	double raioFolha = 200;
	double raioTronco = 100;
	// Composto de uma esfera e um cone
	Arvore arvore (alturaArv, raioFolha, raioTronco, mtFolha, mtTronco);

	// Objeto Complexo de Pinheiro *********************************************
	double hFolhaPin = 400;
	double hTroncoPin = 200;
	double rFolhaPin = 200;
	double rTroncoPin = 40;
	// Composto de uma esfera e um cone
	Pinheiro pinheiro (hFolhaPin, rFolhaPin, hTroncoPin, rTroncoPin, mtFolha, mtTronco);

	// Objeto Complexo do Poste ************************************************
	double alturaVer = 600;
	double alturaHor = 100;
	double raioVer = 12;
	double raioHor = 6;
	// Composto de dois cilindro
	Poste poste (alturaVer, alturaHor, raioVer, raioHor, mtPoste);

	// Objeto Complexo da Lampada **********************************************
	double raioLampada = 10;
	double raioSuporte = 5;
	double alturaSuporte = 5;
	// Composto de uma esfera e um cilindro
	Lampada lampada (raioLampada, raioSuporte, alturaSuporte, mtLampada, mtSuporte);

	// Textura *****************************************************************
	// Carregando a imagens das texturas 
	SDL_Surface* img_door = SDL_LoadBMP("img/door_800x1600.bmp"); 
	SDL_Surface* img_sky = SDL_LoadBMP("img/sky_5960x2734.bmp"); 
	Ponto esq_inf { 0, 0, 0 }; // Define o ponto inferior esquerdo 
	Ponto dir_inf { 1, 0, 0 }; // Define o ponto inferior direito 
	Ponto dir_sup { 1, 1, 0 }; // Define o ponto superior direito 
	Ponto esq_sup { 0, 1, 0 }; // Define o ponto superior esquerdo 
	// Definindo textura da porta
	Textura txt_door (esq_inf, dir_inf, dir_sup, esq_sup, img_door);
	// Definindo textura do ceu
	Textura txt_sky1 (esq_inf, dir_inf, dir_sup, esq_sup, img_sky);
	// Definindo textura do ceu
	Textura txt_sky2 (esq_inf, dir_inf, dir_sup, esq_sup, img_sky);

	// #########################################################################
	// Fontes de luz ###########################################################
	//
	// Informacoes da Fonte Pontual ********************************************
	Vetor intensePontual = { 0.4, 0.4, 0.4 }; // Intensidade da fonte pontual
	Ponto posicaoPontual = { 0, 0, 0 }; // Posicao da fonte pontual
	// Objeto da fonte de luz Pontual
	Pontual pontual (intensePontual, posicaoPontual);

	// Informacoes da Fonte Spot ***********************************************
	Vetor intenseSpot = { 0.4, 0.4, 0.4 }; 
	Ponto posicaoSpot = { 0, 0, 0 };
	Vetor direcaoSpot = { 0, -1, 0 };
	double anguloSpot = (3 * PI) / 8.0;
	// Objeto da fonte de luz Spot
	Spot spot (intenseSpot, posicaoSpot, direcaoSpot, anguloSpot);

	// Informacoes da Fonte Direcional *****************************************
	Vetor intenseDirecional = { 1.0, 1.0 , 1.0 };
	Vetor direcaoDirecional = { -1, -1, -1 };
	// Objeto da fonte de luz Direcional
	Direcional direcional (intenseDirecional, direcaoDirecional);

    // Informacoes da luz Ambiente *********************************************
    Vetor luzAmbiente { 0.3, 0.3, 0.3 };

	// Abrindo arquivo para obter as informacoes
	Info info ("info/info.txt");

	// Informacoes da Janela ***************************************************
	double xminJanela = info.success ? info.xmin : -100; // Valor da parte de cima da janela em cm
	double xmaxJanela = info.success ? info.xmax : 100; // Valor da parte de baixo da janela em cm
	double yminJanela = info.success ? info.ymin : -50; // Valor do lado esquerdo da janela em cm
	double ymaxJanela = info.success ? info.ymax : 50; // Valor do lado direito da janela em cm
	double dJanela = info.success ? info.distance : 100; // Distancia da janela em cm
	// Objeto da Janela
	Janela janela (xminJanela, xmaxJanela, yminJanela, ymaxJanela, dJanela); 

	// Informacoes da Camera ***************************************************
	// Vista em Diagonal
	Ponto eye = info.success ? info.eye : Ponto{ 2600, 1000, 10000 };
	Ponto at = info.success ? info.at : Ponto{ 2600, 0, 0 };
	Ponto up = info.success ? info.up : Ponto{ 2600, 500, 0 }; 
	// Objeto da Camera 
	Camera camera (eye, at, up);

	// Lista de Objetos da Cena ************************************************
	Lista<Objeto> cenario;

    // Definindo a lista de objetos da cena
    void definirObjetos() 
    {
		// Realizando transformacoes *******************************************
		// Ajustando objeto da Casa
		Transformacao::escala(&casa, { 500, 300, 400 });
		Transformacao::translacao(&casa, { 2500, 0, 2000 });

		// Ajustando objeto da Arvore
		Transformacao::translacao(&arvore, { 2200, 0, 2200 });
		// Ajustando objeto do Pinheiro
		Transformacao::translacao(&pinheiro, { 3300, 0, 2200 });
		
		// Ajustando objeto do Poste
		Transformacao::rotacaoY(&poste, PI / 2);
		Transformacao::translacao(&poste, { 2500, 0, 2800 });
		
		// Ajustando objeto da Lampada
		Transformacao::rotacaoX(&lampada, - PI / 2);
		Transformacao::translacao(&lampada, { 2500 + 250, 300, 2000 });
		// Transformacao::translacao(&lampada, { 2500, 100, 2400 });

		// Ajustando objeto da textura
		if (img_door) 
		{
			double largura, altura;
			altura = img_door->h;
			largura = img_door->w;

			Transformacao::escala(&txt_door, { 150, 150 / (largura / altura), 0 });
			Transformacao::translacao(&txt_door, { 2500 + 165, 0, 2400 + 1 });
			
			cenario.add(&txt_door);
		} 
		else SDL_Log("Erro ao abrir imagem da porta da textura! SDL_Error: %s", SDL_GetError());

		if (img_sky) 
		{
			Transformacao::escala(&txt_sky1, { 14900, 7000, 0 });

			Transformacao::escala(&txt_sky2, { 14900, 7000, 0 });
			Transformacao::rotacaoY(&txt_sky2, PI / 2);
			Transformacao::translacao(&txt_sky2, { 0, 0, 14900 });

			cenario.add(&txt_sky1);
			cenario.add(&txt_sky2);
		} 
		else SDL_Log("Erro ao abrir imagem do ceu da textura! SDL_Error: %s", SDL_GetError());

		// Definindo pontos no eixo do Poste
		Ponto p1 { 2500, 100, 2800 }, p2 { 2500, 0, 2800 };
		// Rotacionando objetos em torno do Poste
		Transformacao::rotacaoArbitrario(&arvore, p1, p2, PI / 4);
		Transformacao::rotacaoArbitrario(&pinheiro, p1, p2, PI / 4);
		Transformacao::rotacaoArbitrario(&casa, p1, p2, PI / 4);
		Transformacao::rotacaoArbitrario(&lampada, p1, p2, PI / 4);
		if (img_door) Transformacao::rotacaoArbitrario(&txt_door, p1, p2, PI / 4);

        // Adicionando os objetos na cena **************************************
		cenario.add(&arvore);
		cenario.add(&pinheiro);
		cenario.add(&casa); 
		cenario.add(&poste);
		cenario.add(&lampada);
        cenario.add(&chao);
        // cenario.add(&fundoesq);
        // cenario.add(&fundodir);
    }

	// Lista de Fontes de luz **************************************************
	Lista<Fonte> fontes;

    // Definindo a lista de fontes da cena
    void definirFontes() 
    {
		// Realizando transformacoes *******************************************
		// Spot 
		// Posicionando fonte de luz Spot do Poste
		Vetor translacao = { 2500 + alturaHor, alturaVer - (2 * raioHor) - 1, 2800 };
		Transformacao::translacao(&spot, translacao);

		// Pontual
		// Posicionando fonte de luz Pontual da Casa
		translacao = { 2500 + 250, 300, 2000 - (alturaSuporte + 2 * raioLampada) - 100 };
		Transformacao::translacao(&pontual, translacao);

		// Definindo pontos no eixo do Poste
		Ponto p1 { 2500, 100, 2800 }, p2 { 2500, 0, 2800 };
		// Rotacionando objetos em torno do Poste
		Transformacao::rotacaoArbitrario(&pontual, p1, p2, PI / 4);

        // Adicionando as fontes na lista 
        fontes.add(&spot);
        fontes.add(&pontual);
        fontes.add(&direcional); 
    }
} // namespace Cena

#endif