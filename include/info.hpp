#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "basics.hpp"
#include "objetos.hpp"

class Info
{
private:
    std::ifstream file;
    bool success;
public:
    Info(std::string path) 
    {
        file.open(path);

        if (file.is_open())
        {
            success = true;
            Ponto eye, at, up;

            std::string line; getline(file, line);

            while (file)
            {
                if (line == "camera")
                {
                    for (int i = 0; i < 3; ++i)
                    {
                        getline(file, line);
                        std::istringstream linestream (line);
                        std::string temp; linestream >> temp;

                        if(temp == "eye") {
                            
                        }
                    }
                }

                std::cout << line << "\n"; 
                getline(file, line);
            }
        } 
        else {
            success = false;
            std::cout << "Couldn't open file: " << path << '\n';
        }
    }
    ~Info() {}
};