#ifndef CENA_HPP
#define CENA_HPP

#include "objetos.hpp"

namespace Cena
{
	// Informacoes da Esfera ***************************************************
	double raioEsfera = 40; // Raio da esfera em CM
	Ponto centroEsfera { 0, 0, -100 }; // Centro da esfera
	// Propriedades de reflectividade da esfera
	Vetor kaEsfera { 0.7, 0.2, 0.2 }; // Prop. ambiente do material da esfera 
	Vetor kdEsfera = kaEsfera; // Prop. difusa do material da esfera 
	Vetor keEsfera = kaEsfera; // Prop. especular do material da esfera 
	double brilhoEsfera = 10; // Fator de brilho especular
	// Material da esfera
	Material materialEsfera { kaEsfera, kdEsfera, keEsfera, brilhoEsfera };
	// Objeto da Esfera
	Esfera esfera (centroEsfera, raioEsfera, materialEsfera);

	// Informacoes do Cilindro *************************************************
	double raioCilindro = 20; // Raio da base do Cilindro em cm
	Ponto baseCilindro { -20, -20, -100 }; // Centro da base do Cilindro 
	Ponto topoCilindro = { 20, 20, -60 }; // Posicao do topo do Cilindro
	// Propriedades de reflectividade do Cilindro
	Vetor kaCilindro { 0.2, 0.7, 0.7 }; // Prop. ambiente do material do Cilindro 
	Vetor kdCilindro = kaCilindro; // Prop. difusa do material do Cilindro 
	Vetor keCilindro = kaCilindro; // Prop. especular do material do Cilindro 
	double brilhoCilindro = 10; // Fator de brilho especular
	// Material do Cilindro
	Material materialCilindro { kaCilindro, kdCilindro, keCilindro, brilhoCilindro };
	// Objeto do Cilindro
	Cilindro cilindro (baseCilindro, topoCilindro, raioCilindro, materialCilindro);

	// Informacoes do Cone *****************************************************
	double raioCone = 20; // Raio da base do Cone em cm
	Ponto baseCone { 0, 40, -80 }; // Centro da base do Cone 
	Ponto verticeCone { -40, 0, -60 }; // Posicao do Vertice do Cone
	// Propriedades de reflectividade do Cone 
	Vetor kaCone { 0.7, 0.7, 0.2 }; // Prop. ambiente do material do Cone 
	Vetor kdCone = kaCone; // Prop. difusa do material do Cone 
	Vetor keCone = kaCone; // Prop. especular do material do Cone 
	double brilhoCone = 10; // Fator de brilho especular
	// Material do Cone
	Material materialCone { kaCone, kdCone, keCone, brilhoCone };
	// Objeto do Cone
	Cone cone (baseCone, verticeCone, raioCone, materialCone);

	// Informacoes do Plano do Chao ********************************************
	Ponto pontoChao { 0, -raioEsfera, 0 }; // Ponto presente no plano do chao
	Vetor normalChao { 0, 1, 0 }; // Vetor normal ao plano do chao
	// Propriedades de reflectividade do plano do chao
	Vetor kaChao { 0.2, 0.7, 0.2 }; // Prop. ambiente do material do chao 
	Vetor kdChao = kaChao; // Prop. difusa do material do chao 
	Vetor keChao { 0.0, 0.0, 0.0 }; // Prop. especular do material do chao 
	double brilhoChao = 1; // Fator de brilho espescular
	// Material do plano do chao
	Material materialChao { kaChao, kdChao, keChao, brilhoChao };
	// Objeto do Plano do Chao
	Plano chao (pontoChao, normalChao, materialChao);

	// Informacoes do Plano do Fundo *******************************************
	Ponto pontoFundo { 0, 0, -200 }; // Ponto presente no plano do fundo
	Vetor normalFundo { 0, 0, 1 }; // Vetor normal ao plano do fundo
	// Propriedades de reflectividade do plano de fundo
	Vetor kaFundo { 0.3, 0.3, 0.7 }; // Prop. ambiente do material do fundo 
	Vetor kdFundo = kaFundo; // Prop. difusa do material do fundo 
	Vetor keFundo { 0.0, 0.0, 0.0 }; // Prop. especular do material do fundo 
	double brilhoFundo = 1; // Fator de brilho especular
	// Material do plano de fundo
	Material materialFundo { kaFundo, kdFundo, keFundo, brilhoFundo };
	// Objeto do Plano do Fundo
	Plano fundo (pontoFundo, normalFundo, materialFundo);

	// Informacoes da Malha ****************************************************
	Vetor kaMalha { 0.7, 0.2, 0.7 }; // Prop. ambiente do material da Malha
	Vetor kdMalha = kaMalha; // Prop. difusa do material da Malha
	Vetor keMalha = kaMalha; // Prop. especular do material da Malha
	double brilhoMalha = 10; // Fator de brilho especular
	// Material da Malha
	Material materialMalha { kaMalha, kdMalha, keMalha, brilhoMalha };
	// Objeto da Malha
	int numVertices = 8; // Numero de vertices da Malha
	int numArestas = 18; // Numero de arestas da Malha
	int numFaces = 12; // Numero de faces da Malha
	Malha malha (numVertices, numArestas, numFaces, materialMalha);
    
    void definirMalha () 
    {
        // Definindo os vertices da Malha
        malha.setVertice(0, { -30, -30, -50 }); malha.setVertice(1, {  0, -30, -50 });
        malha.setVertice(2, {  0,  0, -50 }); malha.setVertice(3, { -30,  0, -50 }); 
        malha.setVertice(4, { -15, -15, -100 }); malha.setVertice(5, {  15, -15, -100 }); 
        malha.setVertice(6, {  15,  15, -100 }); malha.setVertice(7, { -15,  15, -100 }); 
        // Definindo as arestas da Malha
        malha.setAresta(0, 0, 1); malha.setAresta(1, 1, 2); 
        malha.setAresta(2, 2, 3); malha.setAresta(3, 3, 0); 
        malha.setAresta(4, 0, 4); malha.setAresta(5, 1, 5); 
        malha.setAresta(6, 2, 6); malha.setAresta(7, 3, 7); 
        malha.setAresta(8, 4, 5); malha.setAresta(9, 5, 6); 
        malha.setAresta(10, 6, 7); malha.setAresta(11, 7, 4); 
        malha.setAresta(12, 1, 3); malha.setAresta(13, 2, 5); 
        malha.setAresta(14, 4, 6); malha.setAresta(15, 0, 7); 
        malha.setAresta(16, 3, 6); malha.setAresta(17, 0, 5); 
        // Definindo as faces da Malha
        malha.setFace(0, 3, 0, 12); malha.setFace(1, 1, 2, 12);
        malha.setFace(2, 1, 5, 13); malha.setFace(3, 9, 6, 13);
        malha.setFace(4, 9, 8, 14); malha.setFace(5, 11, 10, 14);
        malha.setFace(6, 4, 3, 15); malha.setFace(7, 7, 11, 15);
        malha.setFace(8, 2, 6, 16); malha.setFace(9, 10, 7, 16);
        malha.setFace(10, 4, 8, 17); malha.setFace(11, 5, 0, 17);
    }
	// Informacoes da Fonte Pontual ********************************************
	Vetor intensePontual { 0.7, 0.7, 0.7 }; // Intensidade da fonte pontual
	Ponto posicaoPontual { 0, 60, -30 }; // Posicao da fonte pontual
	// Objeto da fonte de luz Pontual
	Pontual pontual (intensePontual, posicaoPontual);

	// Informacoes da Fonte Spot ***********************************************
	Vetor intenseSpot { 1.0, 1.0, 1.0 };
	Ponto posicaoSpot { 30, 50, 0 };
	Vetor direcaoSpot { centroEsfera - posicaoSpot };
	double anguloSpot = 3.1415926 / 6;
	// Objeto da fonte de luz Spot
	Spot spot (intenseSpot, posicaoSpot, direcaoSpot, anguloSpot);

	// Informacoes da Fonte Direcional *****************************************
	Vetor intenseDirecional { 0.5, 0.5, 0.5 };
	Vetor direcaoDirecional { 0, -1, 0 };
	// Objeto da fonte de luz Direcional
	Direcional direcional (intenseDirecional, direcaoDirecional);

    // Informacoes da luz Ambiente *********************************************
    Vetor luzAmbiente { 0.3, 0.3, 0.3 };

	// Lista de Objetos da Cena ************************************************
	Lista<Objeto> cena;
    // Definindo a lista de objetos da cena
    void definirCena() 
    {
        // Adicionando os objetos na cena
        cena.add(&esfera);
        cena.add(&cilindro);
        cena.add(&cone);
        cena.add(&malha);
        cena.add(&chao);
        cena.add(&fundo);
    }

	// Lista de Fontes de luz **************************************************
	Lista<Fonte> fontes;
    // Definindo a lista de fontes da cena
    void definirFontes () 
    {
        // Adicionando as fontes na lista
        fontes.add(&pontual);
        fontes.add(&spot);
        fontes.add(&direcional);
    }
} // namespace Cena

#endif