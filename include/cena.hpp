#ifndef CENA_HPP
#define CENA_HPP

#include "objetos.hpp"
#include "transformacoes.hpp"

#define PI 3.14159265359

namespace Cena
{
	// Materiais dos objetos ***************************************************
	
	// Material da Esfera ******************************************************
	// Propriedades de reflectividade da esfera
	Vetor kaEsfera { 0.7, 0.2, 0.2 }; // Prop. ambiente do material da esfera 
	Vetor kdEsfera = kaEsfera; // Prop. difusa do material da esfera 
	Vetor keEsfera = kaEsfera; // Prop. especular do material da esfera 
	double brilhoEsfera = 10; // Fator de brilho especular
	// Material da esfera
	Material materialEsfera { kaEsfera, kdEsfera, keEsfera, brilhoEsfera };
	
	// Material do Cilindro ****************************************************
	// Propriedades de reflectividade do Cilindro
	Vetor kaCilindro { 0.2, 0.7, 0.7 }; // Prop. ambiente do material do Cilindro 
	Vetor kdCilindro = kaCilindro; // Prop. difusa do material do Cilindro 
	Vetor keCilindro = kaCilindro; // Prop. especular do material do Cilindro 
	double brilhoCilindro = 10; // Fator de brilho especular
	// Material do Cilindro
	Material materialCilindro { kaCilindro, kdCilindro, keCilindro, brilhoCilindro };

	// Material do Cone ********************************************************
	// Propriedades de reflectividade do Cone 
	Vetor kaCone { 0.7, 0.7, 0.2 }; // Prop. ambiente do material do Cone 
	Vetor kdCone = kaCone; // Prop. difusa do material do Cone 
	Vetor keCone = kaCone; // Prop. especular do material do Cone 
	double brilhoCone = 10; // Fator de brilho especular
	// Material do Cone
	Material materialCone { kaCone, kdCone, keCone, brilhoCone };

	// Material do Plano do Chao ***********************************************
	// Propriedades de reflectividade do plano do chao
	Vetor kaChao { 0.2, 0.7, 0.2 }; // Prop. ambiente do material do chao 
	Vetor kdChao = kaChao; // Prop. difusa do material do chao 
	Vetor keChao { 0.0, 0.0, 0.0 }; // Prop. especular do material do chao 
	double brilhoChao = 1; // Fator de brilho espescular
	// Material do plano do chao
	Material materialChao { kaChao, kdChao, keChao, brilhoChao };

	// Material do Plano de Fundo **********************************************
	// Propriedades de reflectividade do plano de fundo
	Vetor kaFundo { 0.3, 0.3, 0.7 }; // Prop. ambiente do material do fundo 
	Vetor kdFundo = kaFundo; // Prop. difusa do material do fundo 
	Vetor keFundo { 0.0, 0.0, 0.0 }; // Prop. especular do material do fundo 
	double brilhoFundo = 1; // Fator de brilho especular
	// Material do plano de fundo
	Material materialFundo { kaFundo, kdFundo, keFundo, brilhoFundo };

	// Material do Plano do Teto ***********************************************
	// Propriedades de reflectividade do plano do Teto
	Vetor kaTeto { 0.3, 0.3, 0.7 }; // Prop. ambiente do material do teto 
	Vetor kdTeto = kaTeto; // Prop. difusa do material do teto 
	Vetor keTeto { 0.0, 0.0, 0.0 }; // Prop. especular do material do teto 
	double brilhoTeto = 1; // Fator de brilho especular
	// Material do plano do Teto
	Material materialTeto { kaTeto, kdTeto, keTeto, brilhoTeto };

	// Material do Cubo ********************************************************
	// Propriedades de reflectividade do Cubo
	Vetor kaCubo { 0.7, 0.2, 0.7 }; // Prop. ambiente do material do Cubo
	Vetor kdCubo = kaCubo; // Prop. difusa do material do Cubo
	Vetor keCubo = kaCubo; // Prop. especular do material do Cubo
	double brilhoCubo = 10; // Fator de brilho especular
	// Material do Cubo
	Material materialCubo { kaCubo, kdCubo, keCubo, brilhoCubo };

	// Material da Pista *******************************************************
	// Propriedades de reflectividade da Pista
	Vetor kaPista = { 0.2, 0.2, 0.2 }; // Prop. ambiente do material da Pista
	Vetor kdPista = kaPista; // Prop. difusa do material da Pista
	Vetor kePista = kaPista; // Prop. especular do material da Pista
	double brilhoPista = 1; // Fator de brilho especular
	// Material da Pista
	Material materialPista { kaPista, kdPista, kePista, brilhoPista };

	// Material da Casa *******************************************************
	// Propriedades de reflectividade da Casa
	Vetor kaCasa = { 0.9, 0.9, 0.9 }; // Prop. ambiente do material da Casa
	Vetor kdCasa = kaCasa; // Prop. difusa do material da Casa
	Vetor keCasa = kaCasa; // Prop. especular do material da Casa
	double brilhoCasa = 10; // Fator de brilho especular
	// Material da Casa
	Material materialCasa { kaCasa, kdCasa, keCasa, brilhoCasa };

	// Material do Telhado *******************************************************
	// Propriedades de reflectividade do Telhado
	Vetor kaTelhado = { 0.6, 0.3, 0.2 }; // Prop. ambiente do material do Telhado
	Vetor kdTelhado = kaTelhado; // Prop. difusa do material do Telhado
	Vetor keTelhado = kaTelhado; // Prop. especular do material do Telhado
	double brilhoTelhado = 5; // Fator de brilho especular
	// Material do Telhado
	Material materialTelhado { kaTelhado, kdTelhado, keTelhado, brilhoTelhado };

	// Material do Poste *******************************************************
	// Propriedades de reflectividade do Poste
	Vetor kaPoste = { 0.4, 0.4, 0.4 }; // Prop. ambiente do material do Poste
	Vetor kdPoste = kaPoste; // Prop. difusa do material do Poste
	Vetor kePoste = { 0.5, 0.5, 0.5 }; // Prop. especular do material do Poste
	double brilhoPoste = 5; // Fator de brilho especular
	// Material do Poste
	Material materialPoste { kaPoste, kdPoste, kePoste, brilhoPoste };

	// Objetos da Cena *********************************************************
	// Objetos em coordenadas de mundo onde todos objetos estao no primeiro
	// octante do sistema de coordenadas. O plano do chao é o plano XZ (y = 0)
	// e os planos XY e YZ sao delimitantes de fundo para o cenario

	// Informacoes da Esfera ***************************************************
	double raioEsfera = 40; // Raio da esfera em CM
	Ponto centroEsfera { 140, raioEsfera, 140 }; // Centro da esfera
	// Objeto da Esfera
	Esfera esfera (centroEsfera, raioEsfera, materialEsfera);

	// Informacoes do Cilindro *************************************************
	double raioCilindro = 20; // Raio da base do Cilindro em cm
	Ponto baseCilindro { 50, 0, 150 }; // Centro da base do Cilindro 
	Ponto topoCilindro = { 50, 50, 150 }; // Posicao do topo do Cilindro
	// Objeto do Cilindro
	Cilindro cilindro (baseCilindro, topoCilindro, raioCilindro, materialCilindro);

	// Informacoes do Cone *****************************************************
	double raioCone = 40; // Raio da base do Cone em cm
	Ponto baseCone { 100, 0, 60 }; // Centro da base do Cone 
	Ponto verticeCone { 100, 50, 60 }; // Posicao do Vertice do Cone
	// Objeto do Cone
	Cone cone (baseCone, verticeCone, raioCone, materialCone);

	// Informacoes do Plano do Chao ********************************************
	Ponto pontoChao { 0, 0, 0 }; // Ponto presente no plano do chao
	Vetor normalChao { 0, 1, 0 }; // Vetor normal ao plano do chao
	// Objeto do Plano do Chao
	Plano chao (pontoChao, normalChao, materialChao);

	// Informacoes do Plano do Fundo 1 *****************************************
	Ponto pontoFundo1 { 0, 0, 0 }; // Ponto presente no plano do fundo 1
	Vetor normalFundo1 { 0, 0, 1 }; // Vetor normal ao plano do fundo 1
	// Objeto do Plano do Fundo 1
	Plano fundo1 (pontoFundo1, normalFundo1, materialFundo);

	// Informacoes do Plano do Fundo 2 *****************************************
	Ponto pontoFundo2 { 0, 0, 0 }; // Ponto presente no plano do fundo 2
	Vetor normalFundo2 { 1, 0, 0 }; // Vetor normal ao plano do fundo 2
	// Objeto do Plano do Fundo 2
	Plano fundo2 (pontoFundo2, normalFundo2, materialFundo);

	// Informacoes do Plano do Fundo 2 *****************************************
	Ponto pontoTeto { 0, 500, 0 }; // Ponto presente no plano do fundo 2
	Vetor normalTeto { 0, -1, 0 }; // Vetor normal ao plano do fundo 2
	// Objeto do Plano do Fundo 2
	Plano teto (pontoTeto, normalTeto, materialTeto);

	// Informacoes do Cubo *****************************************************
	int verticesCubo = 8; // Numero de vertices do Cubo
	int arestasCubo = 18; // Numero de arestas do Cubo
	int facesCubo = 12; // Numero de faces do Cubo
	// Objeto do Cubo
	Malha cubo (verticesCubo, arestasCubo, facesCubo, materialCubo);

	// Informacoes do Quadrado *************************************************
	int verticesQuad = 4; // Numero de vertices do Quadrado
	int arestasQuad = 5; // Numero de vertices do Quadrado
	int facesQuad = 2; // Numero de vertices do Quadrado
	// Objeto do Quadrado
	Malha quadrado (verticesQuad, arestasQuad, facesQuad, materialPista);

	// Informacoes da Piramide *************************************************
	int verticesPiram = 5; // Numero de vertices da Piramide
	int arestasPiram = 9; // Numero de vertices da Piramide
	int facesPiram = 6; // Numero de vertices da Piramide
	// Objeto da Piramide
	Malha piramide (verticesPiram, arestasPiram, facesPiram, materialTelhado);

	// Objeto da Casa 1
	Malha casa1 (verticesCubo, arestasCubo, facesCubo, materialCasa);
	// Objeto da Casa 2
	Malha casa2 (verticesCubo, arestasCubo, facesCubo, materialCasa);
	// Objeto da Casa 3
	Malha casa3 (verticesCubo, arestasCubo, facesCubo, materialCasa);
	// Objeto da Casa 4
	Malha casa4 (verticesCubo, arestasCubo, facesCubo, materialCasa);

	// Objeto do Telhado 1
	Malha telhado1 (verticesPiram, arestasPiram, facesPiram, materialTelhado);
	// Objeto do Telhado 2
	Malha telhado2 (verticesPiram, arestasPiram, facesPiram, materialTelhado);
	// Objeto do Telhado 3
	Malha telhado3 (verticesPiram, arestasPiram, facesPiram, materialTelhado);
	// Objeto do Telhado 4
	Malha telhado4 (verticesPiram, arestasPiram, facesPiram, materialTelhado);

	// Objeto da Pista 1
	Malha pista1 (verticesQuad, arestasQuad, facesQuad, materialPista);
	// Objeto da Pista 2
	Malha pista2 (verticesQuad, arestasQuad, facesQuad, materialPista);
	// Objeto da Pista 3
	Malha pista3 (verticesQuad, arestasQuad, facesQuad, materialPista);
	// Objeto da Pista 4
	Malha pista4 (verticesQuad, arestasQuad, facesQuad, materialPista);
	// Objeto da Pista 5
	Malha pista5 (verticesQuad, arestasQuad, facesQuad, materialPista);
	// Objeto da Pista 6
	Malha pista6 (verticesQuad, arestasQuad, facesQuad, materialPista);

	// Informacoes do Poste ****************************************************
	double raioPoste = 0.5; // Raio da base do Poste em cm
	Ponto basePoste { 0, 0, 0 }; // Centro da base do Poste 
	Ponto topoPoste = { 0, 25, 0 }; // Posicao do topo do Poste
	// Objeto do Poste 1
	Cilindro poste1 (basePoste, topoPoste, raioPoste, materialPoste);
	// Objeto do Poste 2
	Cilindro poste2 (basePoste, topoPoste, raioPoste, materialPoste);
	// Objeto do Poste 3
	Cilindro poste3 (basePoste, topoPoste, raioPoste, materialPoste);
	// Objeto do Poste 4
	Cilindro poste4 (basePoste, topoPoste, raioPoste, materialPoste);

	// Informacoes da Haste ****************************************************
	double raioHaste = 0.25; // Raio da base da Haste em cm
	Ponto baseHaste { 0, 0, 0 }; // Centro da base da Haste 
	Ponto topoHaste = { 5, 0, 0 }; // Posicao do topo da Haste
	// Objeto da Haste 1
	Cilindro haste1 (baseHaste, topoHaste, raioHaste, materialPoste);
	// Objeto da Haste 2
	Cilindro haste2 (baseHaste, topoHaste, raioHaste, materialPoste);
	// Objeto da Haste 3
	Cilindro haste3 (baseHaste, topoHaste, raioHaste, materialPoste);
	// Objeto da Haste 4
	Cilindro haste4 (baseHaste, topoHaste, raioHaste, materialPoste);

	// Informacoes do Suporte ****************************************************
	double raioSuporte = 0.5; // Raio da base do Suporte em cm
	Ponto baseSuporte { 0, 0, 0 }; // Centro da base do Suporte 
	Ponto verticeSuporte = { 0, 1, 0 }; // Posicao do topo do Suporte
	// Objeto do Suporte 1
	Cone suporte1 (baseSuporte, verticeSuporte, raioSuporte, materialPoste);
	// Objeto do Suporte 2
	Cone suporte2 (baseSuporte, verticeSuporte, raioSuporte, materialPoste);
	// Objeto do Suporte 3
	Cone suporte3 (baseSuporte, verticeSuporte, raioSuporte, materialPoste);
	// Objeto do Suporte 4
	Cone suporte4 (baseSuporte, verticeSuporte, raioSuporte, materialPoste);

	void definirMalha() 
	{
        // Definindo os vertices da Piramide ***********************************
        piramide.setVertice(0, { 0, 0, 10 }); piramide.setVertice(1, { 10, 0, 10 });
        piramide.setVertice(2, { 10, 0, 0 }); piramide.setVertice(3, { 0, 0, 0 }); 
		piramide.setVertice(4, { 5, 5, 5 });
		// Definindo as arestas da Piramide
        piramide.setAresta(0, 0, 1); piramide.setAresta(1, 1, 2); 
		piramide.setAresta(2, 2, 3); piramide.setAresta(3, 3, 0); 
        piramide.setAresta(4, 0, 4); piramide.setAresta(5, 1, 4); 
		piramide.setAresta(6, 2, 4); piramide.setAresta(7, 3, 4); 
		piramide.setAresta(8, 0, 2);
		// Definindo as faces da Piramide
		piramide.setFace(0, 0, 5, 4); piramide.setFace(1, 1, 6, 5); 
		piramide.setFace(2, 2, 7, 6); piramide.setFace(3, 3, 4, 7); 
		piramide.setFace(4, 1, 0, 8); piramide.setFace(5, 3, 2, 8);
		
        // Definindo os vertices do Quadrado ***********************************
        quadrado.setVertice(0, { 0, 0.1, 10 }); quadrado.setVertice(1, { 10, 0.1, 10 });
        quadrado.setVertice(2, { 10, 0.1, 0 }); quadrado.setVertice(3, { 0, 0.1, 0 }); 
		// Definindo as arestas do Quadrado
        quadrado.setAresta(0, 0, 1); quadrado.setAresta(1, 1, 2); 
		quadrado.setAresta(2, 2, 3); quadrado.setAresta(3, 3, 0); 
		quadrado.setAresta(4, 0, 2); 
		// Definindo as faces do Quadrado
		quadrado.setFace(0, 0, 1, 4); quadrado.setFace(1, 2, 3, 4); 

        // Definindo os vertices do Cubo ***************************************
        cubo.setVertice(0, { 0, 0, 10 }); cubo.setVertice(1, { 10, 0, 10 });
        cubo.setVertice(2, { 10, 10, 10 }); cubo.setVertice(3, { 0, 10, 10 }); 
        cubo.setVertice(4, { 0, 0, 0 }); cubo.setVertice(5, { 10, 0, 0 }); 
        cubo.setVertice(6, { 10, 10, 0 }); cubo.setVertice(7, { 0, 10, 0 }); 
        // Definindo as arestas do Cubo
        cubo.setAresta(0, 0, 1); cubo.setAresta(1, 1, 2); 
        cubo.setAresta(2, 2, 3); cubo.setAresta(3, 3, 0); 
        cubo.setAresta(4, 0, 4); cubo.setAresta(5, 1, 5); 
        cubo.setAresta(6, 2, 6); cubo.setAresta(7, 3, 7); 
        cubo.setAresta(8, 4, 5); cubo.setAresta(9, 5, 6); 
        cubo.setAresta(10, 6, 7); cubo.setAresta(11, 7, 4); 
        cubo.setAresta(12, 1, 3); cubo.setAresta(13, 2, 5); 
        cubo.setAresta(14, 4, 6); cubo.setAresta(15, 0, 7); 
        cubo.setAresta(16, 3, 6); cubo.setAresta(17, 0, 5); 
        // Definindo as faces do Cubo
        cubo.setFace(0, 3, 0, 12); cubo.setFace(1, 1, 2, 12);
        cubo.setFace(2, 1, 5, 13); cubo.setFace(3, 9, 6, 13);
        cubo.setFace(4, 9, 8, 14); cubo.setFace(5, 11, 10, 14);
        cubo.setFace(6, 4, 3, 15); cubo.setFace(7, 7, 11, 15);
        cubo.setFace(8, 2, 6, 16); cubo.setFace(9, 10, 7, 16);
        cubo.setFace(10, 4, 8, 17); cubo.setFace(11, 5, 0, 17);
    }

	// Fontes de luz ***********************************************************
	//
	// Informacoes da Fonte Pontual ********************************************
	Vetor intensePontual = { 1.0, 1.0, 1.0 }; // Intensidade da fonte pontual
	Ponto posicaoPontual = { 100, 100, 50 }; // Posicao da fonte pontual
	// Objeto da fonte de luz Pontual
	Pontual pontual (intensePontual, posicaoPontual);

	// Informacoes da Fonte Spot ***********************************************
	Vetor intenseSpot = { 0.3, 0.3, 0.3 };
	Ponto posicaoSpot = { 0, 0, 0 };
	Vetor direcaoSpot = { 0, -1, 0 };
	double anguloSpot = PI / 3;
	// Objeto da fonte de luz Spot
	Spot spot1 (intenseSpot, posicaoSpot, direcaoSpot, anguloSpot);
	// Objeto da fonte de luz Spot
	Spot spot2 (intenseSpot, posicaoSpot, direcaoSpot, anguloSpot);
	// Objeto da fonte de luz Spot
	Spot spot3 (intenseSpot, posicaoSpot, direcaoSpot, anguloSpot);
	// Objeto da fonte de luz Spot
	Spot spot4 (intenseSpot, posicaoSpot, direcaoSpot, anguloSpot);

	// Informacoes da Fonte Direcional *****************************************
	Vetor intenseDirecional = { 0.5, 0.5, 0.5 };
	Vetor direcaoDirecional = { -1, -1, -1 };
	// Objeto da fonte de luz Direcional
	Direcional direcional (intenseDirecional, direcaoDirecional);

    // Informacoes da luz Ambiente *********************************************
    Vetor luzAmbiente { 0.2, 0.2, 0.2 };

	// Lista de Objetos da Cena ************************************************
	Lista<Objeto> cenario;
    // Definindo a lista de objetos da cena
    void definirCena() 
    {
		// Realizando transformacoes nos objetos *******************************
		//
		// Ajustando tamanho e local da pista 1
		quadrado.copiar(&pista1);
		Transformacao::escala(&pista1, { 10, 1, 1 });
		Transformacao::translacao(&pista1, { 0, 0, 20 });
		// Ajustando tamanho e local da pista 2
		pista1.copiar(&pista2);
		Transformacao::translacao(&pista2, { 0, 0, 20 });
		// Ajustando tamanho e local da pista 3
		pista2.copiar(&pista3);
		Transformacao::translacao(&pista3, { 0, 0, 20 });
		// Ajustando tamanho e local da pista 4
		quadrado.copiar(&pista4);
		Transformacao::escala(&pista4, { 1, 1, 10 });
		Transformacao::translacao(&pista4, { 20, 0, 0 });
		// Ajustando tamanho e local da pista 5
		pista4.copiar(&pista5);
		Transformacao::translacao(&pista5, { 20, 0, 0 });
		// Ajustando tamanho e local da pista 6
		pista5.copiar(&pista6);
		Transformacao::translacao(&pista6, { 20, 0, 0 });

		// Ajustando local das casas
		cubo.copiar(&casa1); cubo.copiar(&casa2); 
		cubo.copiar(&casa3); cubo.copiar(&casa4);
		Transformacao::translacao(&casa1, { 30, 0, 30 });
		Transformacao::translacao(&casa2, { 50, 0, 30 });
		Transformacao::translacao(&casa3, { 30, 0, 50 });
		Transformacao::translacao(&casa4, { 50, 0, 50 });
		// Ajustando local dos telhados
		piramide.copiar(&telhado1); piramide.copiar(&telhado2);
		piramide.copiar(&telhado3); piramide.copiar(&telhado4);  
		Transformacao::translacao(&telhado1, { 30, 10, 30 });
		Transformacao::translacao(&telhado2, { 50, 10, 30 });
		Transformacao::translacao(&telhado3, { 30, 10, 50 });
		Transformacao::translacao(&telhado4, { 50, 10, 50 });
		// Ajustando local dos postes
		Transformacao::translacao(&poste1, { 20, 0, 20 });
		Transformacao::translacao(&poste2, { 70, 0, 20 });
		Transformacao::translacao(&poste3, { 20, 0, 70 });
		Transformacao::translacao(&poste4, { 70, 0, 70 });
		// Ajustando direcao e local das hastes
		Transformacao::rotacaoY(&haste1, -PI / 4);
		Transformacao::translacao(&haste1, { 20, 24.75, 20 });
		Transformacao::rotacaoY(&haste2, -(3 * PI) / 4);
		Transformacao::translacao(&haste2, { 70, 24.75, 20 });
		Transformacao::rotacaoY(&haste3, PI / 4);
		Transformacao::translacao(&haste3, { 20, 24.75, 70 });
		Transformacao::rotacaoY(&haste4, (3 * PI) / 4);
		Transformacao::translacao(&haste4, { 70, 24.75, 70 });

		Transformacao::translacao(&suporte1, { 23.5, 23.5, 23.5 });
		Transformacao::translacao(&suporte2, { 66.5, 23.5, 23.5 });
		Transformacao::translacao(&suporte3, { 23.5, 23.5, 66.5 });
		Transformacao::translacao(&suporte4, { 66.5, 23.5, 66.5 });

		Transformacao::translacao(&spot1, { 23.5, 23, 23.5 });
		Transformacao::translacao(&spot2, { 66.5, 23, 23.5 });
		Transformacao::translacao(&spot3, { 23.5, 23, 66.5 });
		Transformacao::translacao(&spot4, { 66.5, 23, 66.5 });

        // Adicionando os objetos na cena **************************************
        // cenario.add(&esfera);
        // cenario.add(&cilindro);
        // cenario.add(&cone);
        // cenario.add(&cubo);
		// cenario.add(&quadrado);
		// cenario.add(&piramide);

		// Adicionando objetos das pistas
		cenario.add(&pista1); cenario.add(&pista2); cenario.add(&pista3);
		cenario.add(&pista4); cenario.add(&pista5); cenario.add(&pista6);
		// Adicionando objetos das casas
		cenario.add(&casa1); cenario.add(&casa2); 
		cenario.add(&casa3); cenario.add(&casa4);
		// Adicionando objetos dos telhados
		cenario.add(&telhado1); cenario.add(&telhado2); 
		cenario.add(&telhado3); cenario.add(&telhado4);
		// Adicionando objetos dos postes
		cenario.add(&poste1);
		cenario.add(&poste2);
		cenario.add(&poste3);
		cenario.add(&poste4);
		// Adicionando objetos das hastes
		cenario.add(&haste1);
		cenario.add(&haste2);
		cenario.add(&haste3);
		cenario.add(&haste4);
		// Adicionando objetos dos suportes
		cenario.add(&suporte1);
		cenario.add(&suporte2);
		cenario.add(&suporte3);
		cenario.add(&suporte4);
		// Adicionando planos limitantes
        cenario.add(&chao);
        cenario.add(&fundo1);
        cenario.add(&fundo2);
		// cenario.add(&teto);
    }

	// Lista de Fontes de luz **************************************************
	Lista<Fonte> fontes;
    // Definindo a lista de fontes da cena
    void definirFontes() 
    {
        // Adicionando as fontes na lista
        fontes.add(&spot1);
        fontes.add(&spot2);
        fontes.add(&spot3);
        fontes.add(&spot4);
        // fontes.add(&pontual);
        fontes.add(&direcional);
    }
} // namespace Cena

#endif