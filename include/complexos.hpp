#ifndef COMPLEXOS_HPP
#define COMPLEXOS_HPP

#include <iostream>
#include "objetos.hpp"

class Casa : public Objeto
{
private:
    Malha *telhado, *parede;
    Malha* atingido = nullptr;
public:
    Casa(Material mp, Material mt) 
    {
        parede = new Malha (8, 18, 12, mp);
        telhado = new Malha (5, 9, 6, mt);

        // Definindo os vertices da Parede *************************************
        parede->setVertice(0, { 0, 0, 10 }); parede->setVertice(1, { 10, 0, 10 });
        parede->setVertice(2, { 10, 10, 10 }); parede->setVertice(3, { 0, 10, 10 }); 
        parede->setVertice(4, { 0, 0, 0 }); parede->setVertice(5, { 10, 0, 0 }); 
        parede->setVertice(6, { 10, 10, 0 }); parede->setVertice(7, { 0, 10, 0 }); 
        // Definindo as arestas da Parede
        parede->setAresta(0, 0, 1); parede->setAresta(1, 1, 2); 
        parede->setAresta(2, 2, 3); parede->setAresta(3, 3, 0); 
        parede->setAresta(4, 0, 4); parede->setAresta(5, 1, 5); 
        parede->setAresta(6, 2, 6); parede->setAresta(7, 3, 7); 
        parede->setAresta(8, 4, 5); parede->setAresta(9, 5, 6); 
        parede->setAresta(10, 6, 7); parede->setAresta(11, 7, 4); 
        parede->setAresta(12, 1, 3); parede->setAresta(13, 2, 5); 
        parede->setAresta(14, 4, 6); parede->setAresta(15, 0, 7); 
        parede->setAresta(16, 3, 6); parede->setAresta(17, 0, 5); 
        // Definindo as faces da Parede
        parede->setFace(0, 3, 0, 12); parede->setFace(1, 1, 2, 12);
        parede->setFace(2, 1, 5, 13); parede->setFace(3, 9, 6, 13);
        parede->setFace(4, 9, 8, 14); parede->setFace(5, 11, 10, 14);
        parede->setFace(6, 4, 3, 15); parede->setFace(7, 7, 11, 15);
        parede->setFace(8, 2, 6, 16); parede->setFace(9, 10, 7, 16);
        parede->setFace(10, 4, 8, 17); parede->setFace(11, 5, 0, 17);

        // Definindo os vertices do telhado ************************************
        telhado->setVertice(0, { 0, 0, 10 }); telhado->setVertice(1, { 10, 0, 10 });
        telhado->setVertice(2, { 10, 0, 0 }); telhado->setVertice(3, { 0, 0, 0 }); 
		telhado->setVertice(4, { 5, 5, 5 });
		// Definindo as arestas do telhado
        telhado->setAresta(0, 0, 1); telhado->setAresta(1, 1, 2); 
		telhado->setAresta(2, 2, 3); telhado->setAresta(3, 3, 0); 
        telhado->setAresta(4, 0, 4); telhado->setAresta(5, 1, 4); 
		telhado->setAresta(6, 2, 4); telhado->setAresta(7, 3, 4); 
		telhado->setAresta(8, 0, 2);
		// Definindo as faces do telhado
		telhado->setFace(0, 0, 5, 4); telhado->setFace(1, 1, 6, 5); 
		telhado->setFace(2, 2, 7, 6); telhado->setFace(3, 3, 4, 7); 
		telhado->setFace(4, 1, 0, 8); telhado->setFace(5, 3, 2, 8);
    }
    ~Casa () { delete parede; delete telhado; }

    double intersecao (Raio raio) override
    {
        std::cout << "Abrindo intersecao...\n";
        double t_parede = parede->intersecao(raio);
        double t_telhado = telhado->intersecao(raio);

        if (t_parede <= 0) 
        {
            atingido = telhado;
            material = telhado->material;
            return t_telhado;
        } 
        else if (t_telhado <= 0) 
        {
            atingido = parede;
            material = parede->material;
            return t_parede;
        }
        else if (t_parede < t_telhado)
        {
            atingido = parede;
            material = parede->material;
            return t_parede;
        }
        else {
            atingido = telhado;
            material = telhado->material;
            return t_telhado;
        }
        std::cout << "Fechando intersecao...\n";
    }

    Vetor getNormal (Ponto ponto) const override
    {
        std::cout << "Abrindo getNormal...\n";
        if (atingido == telhado)
            return telhado->getNormal(ponto);
        else 
            return parede->getNormal(ponto);
    }

    void transformar (Matriz matriz) override
    {
        telhado->transformar(matriz);
        parede->transformar(matriz);
    }
};

class Arvore : public Objeto
{
private:
    Cone* tronco;
    Esfera* folha;
    Objeto* atingido;
public:
    Arvore (Material mf, Material mt) 
    {
        std::cout << "Criando arvore...\n";
        folha = new Esfera ({ 0, 0, 0 }, 150, mf);
        tronco = new Cone({ 0, 0, 0 }, { 0, 300, 0 }, 75, mt);
        std::cout << "Arvore criada...\n";
    }
    ~Arvore () { delete folha; delete tronco; }

    double intersecao (Raio raio) override
    {
        double t_folha = folha->intersecao(raio);
        double t_tronco = tronco->intersecao(raio);

        if (t_folha <= 0) 
        {
            atingido = tronco;
            material = tronco->material;
            return t_tronco;
        } 
        else if (t_tronco <= 0) 
        {
            atingido = folha;
            material = folha->material;
            return t_folha;
        }
        else if (t_folha < t_tronco)
        {
            atingido = folha;
            material = folha->material;
            return t_folha;
        }
        else {
            atingido = tronco;
            material = tronco->material;
            return t_tronco;
        }
    }

    Vetor getNormal (Ponto ponto) const override
    {
        if (atingido == folha)
            return folha->getNormal(ponto);
        else 
            return tronco->getNormal(ponto);
    }

    void transformar (Matriz matriz) override
    {
        folha->transformar(matriz);
        tronco->transformar(matriz);
    }
};

#endif