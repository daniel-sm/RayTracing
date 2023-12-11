#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "objetos.hpp"
#include "transformacoes.hpp"
#include "cena.hpp"

class InfoTransform 
{
private:
    std::ifstream file;

    void getTransformation (std::string line, Objeto* obj) 
    {
        std::istringstream linestream (line);
        std::string transform; 
        linestream >> transform;

        while (line != "-")
        {
            if (transform == "translacao")
            {
                Vetor t;
                linestream >> t.x;
                linestream >> t.y;
                linestream >> t.z;

                Transformacao::translacao(obj, t);
            } 
            else if (transform == "escala")
            {
                Vetor e;
                linestream >> e.x;
                linestream >> e.y;
                linestream >> e.z;

                Transformacao::escala(obj, e);
            } 
            else if (transform == "rotacao")
            {
                std::string rotacao;
                linestream >> rotacao; 

                double angulo;

                if (rotacao == "X")
                {
                    linestream >> angulo;
                    Transformacao::rotacaoX(obj, angulo);
                } 
                else if (rotacao == "Y")
                {
                    linestream >> angulo;
                    Transformacao::rotacaoY(obj, angulo);

                } else if (rotacao == "Z")
                {
                    linestream >> angulo;
                    Transformacao::rotacaoZ(obj, angulo);

                } else if (rotacao == "A")
                {
                    Ponto p1, p2;

                    for (int i = 0; i <= 3; ++i)
                    {
                        getline(file, line);

                        std::istringstream newlinestream (line);

                        std::string aux;
                        newlinestream >> aux; 

                        if (aux == "p1") newlinestream >> p1.x >> p1.y >> p1.z;
                        else if (aux == "p2") newlinestream >> p2.x >> p2.y >> p2.z;
                        else if (aux == "angulo") newlinestream >> angulo; 
                    }

                    Transformacao::rotacaoArbitrario(obj, p1, p2, angulo);
                }
            } 
            else if (transform == "espelho")
            {
                std::string tipo;
                linestream >> tipo;

                if (tipo == "XY") Transformacao::espelhoXY(obj); 
                else if (tipo == "YZ") Transformacao::espelhoYZ(obj); 
                else if (tipo == "XZ") Transformacao::espelhoXZ(obj);
                else if (tipo == "A") 
                {
                    Ponto p;
                    Vetor n;

                    for (int i = 0; i <= 2; ++i)
                    {
                        getline(file, line);

                        std::istringstream newlinestream (line);

                        std::string aux;
                        newlinestream >> aux; 

                        if (aux == "ponto") newlinestream >> p.x >> p.y >> p.z;
                        else if (aux == "normal") newlinestream >> n.x >> n.y >> n.z;
                    }

                    Transformacao::espelhoArbitrario(obj, p, n);
                }
            } 
            else if (transform == "cisalhamento")
            {
                std::string tipo;
                linestream >> tipo;

                double angulo;
                linestream >> angulo;

                if (tipo == "XY") Transformacao::cisalhamentoXY(obj, angulo);
                else if (tipo == "YX") Transformacao::cisalhamentoYX(obj, angulo);
                else if (tipo == "YZ") Transformacao::cisalhamentoYZ(obj, angulo);
                else if (tipo == "ZY") Transformacao::cisalhamentoZY(obj, angulo);
                else if (tipo == "XZ") Transformacao::cisalhamentoXZ(obj, angulo);
                else if (tipo == "ZX") Transformacao::cisalhamentoZX(obj, angulo);
            } 
            else if (transform == "remove") { Cena::cenario.remove(obj); }

            getline(file, line);
        }
    }
public:
    bool success;

    InfoTransform (std::string path)
    {
        file.open(path);

        if (file.is_open())
        {
            success = true; // salvando que o arquivo foi lido com sucesso 
            std::string line; // string que recebera o valor de cada linha

            // lendo a primeira linha
            getline(file, line);

            while (file)
            {
                if (line == "casa")
                {
                    getline(file, line);
                    getTransformation(line, &(Cena::casa));
                } 
                else if (line == "arvore")
                {
                    getline(file, line);
                    getTransformation(line, &(Cena::arvore));
                } else if (line == "pinheiro")
                {
                    getline(file, line);
                    getTransformation(line, &(Cena::pinheiro));
                } else if (line == "poste")
                {
                    getline(file, line);
                    getTransformation(line, &(Cena::poste));
                } else if (line == "porta")
                {
                    getline(file, line);
                    getTransformation(line, &(Cena::txt_door));
                } else if (line == "lampada")
                {
                    getline(file, line);
                    getTransformation(line, &(Cena::lampada));
                } else if (line == "chao")
                {
                    getline(file, line);
                    getTransformation(line, &(Cena::chao));
                } else if (line == "fundo1")
                {
                    getline(file, line);
                    getTransformation(line, &(Cena::txt_sky1));
                } else if (line == "fundo2")
                {
                    getline(file, line);
                    getTransformation(line, &(Cena::txt_sky2));
                }
                getline(file, line);
            }
        }
        else {
            success = false; // salvando que o arquivo nao foi lido  
            std::cout << "Couldn't open file: " << path << "\n";
        }
    }
};