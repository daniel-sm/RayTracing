#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "basics.hpp"
// #include "objetos.hpp"

class Info
{
private:
    std::ifstream file;
public:
    Ponto eye, at, up;
    double xmin, xmax, ymin, ymax, distance;
    int projecao;
    Vetor dir_projecao;
    bool success;
    
    Info(std::string path) 
    {
        file.open(path);

        if (file.is_open())
        {
            success = true; // guarda que o arquivo foi lido com sucesso 
            std::string line; // string que recebera o valor de cada linha

            // lendo a primeira linha
            getline(file, line);

            while (file) // enquanto o arquivo nao terminar ou nao ter erro
            {
                if (line == "camera") // lendo dados da camera
                {
                    for (int i = 0; i <= 3; ++i)
                    {
                        // lendo as informacoes das proximas linhas
                        getline(file, line);

                        std::istringstream linestream (line);
                        std::string aux; linestream >> aux;

                        if(aux == "eye") linestream >> eye.x >> eye.y >> eye.z; 
                        if (aux == "at") linestream >> at.x >> at.y >> at.z; 
                        if (aux == "up") linestream >> up.x >> up.y >> up.z; 
                    }
                }
                if (line == "janela") // lendo dados da janela
                {
                    for (int i = 0; i <= 3; ++i)
                    {
                        // lendo as informacoes das proximas linhas
                        getline(file, line);

                        std::istringstream linestream (line);
                        std::string aux; linestream >> aux;

                        if(aux == "x") linestream >> xmin >> xmax; 
                        if (aux == "y") linestream >> ymin >> ymax; 
                        if (aux == "d") linestream >> distance;
                    }
                }
                if (line == "projecao")
                {
                    // lendo as informacoes das proximas linhas
                    getline(file, line);

                    std::istringstream linestream (line);
                    std::string aux; linestream >> aux;
                    
                    if (aux == "per") { projecao = 1; dir_projecao = {0, 0, 0}; }
                    if (aux == "ort") { projecao = 2; dir_projecao = {0, 0, 0}; }
                    if (aux == "obl") 
                    { 
                        projecao = 3; 
                        linestream >> dir_projecao.x >> dir_projecao.y >> dir_projecao.z; 
                    }
                }
                // lendo proxima linha do arquivo
                getline(file, line);
            }
        } 
        else {
            success = false; // define que o arquivo nao foi lido com sucesso
            std::cout << "Couldn't open file: " << path << '\n';
        }

        // std::cout << "eye " << eye.x << " " << eye.y << " " << eye.z << '\n';
        // std::cout << "at " << at.x << " " << at.y << " " << at.z << '\n';
        // std::cout << "up " << up.x << " " << up.y << " " << up.z << '\n';

        // std::cout << "xmin " << xmin << " xmax " << xmax << '\n';
        // std::cout << "ymin " << ymin << " ymax " << ymax << '\n';
        // std::cout << "d: " << distance;

        std::cout << "proj " << projecao << '\n';
        std::cout << "dir " << dir_projecao.x << " " << dir_projecao.y << " " << dir_projecao.z << '\n';
    }
    ~Info() {}
};