#ifndef CENA_HPP
#define CENA_HPP

#include "objetos.hpp"

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

	// Material do Cubo ********************************************************
	// Propriedades de reflectividade do Cubo
	Vetor kaCubo { 0.7, 0.2, 0.7 }; // Prop. ambiente do material do Cubo
	Vetor kdCubo = kaCubo; // Prop. difusa do material do Cubo
	Vetor keCubo = kaCubo; // Prop. especular do material do Cubo
	double brilhoCubo = 10; // Fator de brilho especular
	// Material do Cubo
	Material materialCubo { kaCubo, kdCubo, keCubo, brilhoCubo };

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

	// Informacoes do Cubo *****************************************************
	int numVertices = 8; // Numero de vertices do Cubo
	int numArestas = 18; // Numero de arestas do Cubo
	int numFaces = 12; // Numero de faces do Cubo
	// Objeto do Cubo
	Malha cubo (numVertices, numArestas, numFaces, materialCubo);

    void definirCubo () 
    {
        // Definindo os vertices do Cubo
        cubo.setVertice(0, { 0, 0, 20 }); cubo.setVertice(1, { 20, 0, 20 });
        cubo.setVertice(2, { 20, 20, 20 }); cubo.setVertice(3, { 0, 20, 20 }); 
        cubo.setVertice(4, { 0, 0, 0 }); cubo.setVertice(5, { 20, 0, 0 }); 
        cubo.setVertice(6, { 20, 20, 0 }); cubo.setVertice(7, { 0, 20, 0 }); 
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

	// Informacoes da Fonte Pontual ********************************************
	Vetor intensePontual = { 1.0, 1.0, 1.0 }; // Intensidade da fonte pontual
	Ponto posicaoPontual = { 196, 196, 196 }; // Posicao da fonte pontual
	// Objeto da fonte de luz Pontual
	Pontual pontual (intensePontual, posicaoPontual);

	// Informacoes da Fonte Spot ***********************************************
	Vetor intenseSpot = { 1.0, 1.0, 1.0 };
	Ponto posicaoSpot = { 50, 50, 50 };
	Vetor direcaoSpot = { -50, -50, -50 }; //{ 0, -1, 0 };
	double anguloSpot = 3.1415926 / 6;
	// Objeto da fonte de luz Spot
	Spot spot (intenseSpot, posicaoSpot, direcaoSpot, anguloSpot);

	// Informacoes da Fonte Direcional *****************************************
	Vetor intenseDirecional = { 0.5, 0.5, 0.5 };
	Vetor direcaoDirecional = { 0, -1, 0 };
	// Objeto da fonte de luz Direcional
	Direcional direcional (intenseDirecional, direcaoDirecional);

    // Informacoes da luz Ambiente *********************************************
    Vetor luzAmbiente { 0.2, 0.2, 0.2 };

	// Lista de Objetos da Cena ************************************************
	Lista<Objeto> cenario;
    // Definindo a lista de objetos da cena
    void definirCena() 
    {
        // Adicionando os objetos na cena
        cenario.add(&esfera);
        cenario.add(&cilindro);
        cenario.add(&cone);
        cenario.add(&cubo);
        cenario.add(&chao);
        cenario.add(&fundo1);
        cenario.add(&fundo2);
    }

	// Lista de Fontes de luz **************************************************
	Lista<Fonte> fontes;
    // Definindo a lista de fontes da cena
    void definirFontes() 
    {
        // Adicionando as fontes na lista
        fontes.add(&pontual);
        fontes.add(&spot);
        fontes.add(&direcional);
    }
} // namespace Cena

#endif