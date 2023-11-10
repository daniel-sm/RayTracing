#ifndef MATRIZ_HPP
#define MATRIZ_HPP

#include <vector>

struct ErroMatriz { const char* msg; };

// Classe de Matriz para operacoes matematicas 
//
class Matriz 
{
private:
    std::vector<std::vector<double>> matriz;
    int linhas, colunas;
public:
    Matriz () : linhas{0}, colunas{0} {}
    Matriz (int l, int c) : linhas{l}, colunas{c} 
    {
        // Redimensiona o vetor para uma matriz (l x c) com valores 0.0
        matriz.resize(l, std::vector<double>(c, 0.0)); 
    }
    // Define o tamanho da matriz quando inicializada vazia
    void setSize (int l, int c) 
    {
        linhas = l; // redefine o numero de linhas
        colunas = c; // redefine o numero de colunas
        matriz.resize(linhas, std::vector<double>(colunas, 0.0)); 
    }
    
    // Retorna o numero de linhas da matriz
    int numLinhas () { return linhas; }
    // Retorna o numero de colunas da matriz
    int numColunas () { return colunas; }

    // Acessa um elemento especifico
    double& operator() (int l, int c) { return matriz[l][c]; }

}; // fim class Matriz

// Funcao para gerar uma matriz identidade (n x n)
Matriz identidade (int n) 
{
    Matriz I (n, n);

    for (int i = 0; i < n; ++i) 
    {
        for (int j = 0; j < n; ++j) 
        {
            if (i == j)
                I(i, j) = 1;
            else
                I(i, j) = 0;
        }
    }
    return I;
}

// Funcao que retorna a transposta de uma matriz
Matriz transposta (Matriz matriz) 
{ 
    int l = matriz.numLinhas();
    int c = matriz.numColunas();

    Matriz result (c, l);

    for (int i = 0; i < c; ++i)
        for (int j = 0; j < l; ++j) 
            result(i, j) = matriz(j, i);

    return result;
}

// Operacoes com matrizes
// Para as operacoes assume-se que atende os requisitos de cada operacao
//
// Para SOMA, assume que num de linhas e num de colunas sao iguais 
Matriz operator+ (Matriz primeira, Matriz segunda) 
{
    if (primeira.numLinhas() != segunda.numLinhas() 
        || primeira.numColunas() != segunda.numColunas())
    {
        throw ErroMatriz{"Matrizes incompatíveis com adição!\n"};
    }

    int l = primeira.numLinhas();
    int c = primeira.numColunas();

    Matriz result(l, c);

    for (int i = 0; i < l; ++i) 
        for (int j = 0; j < c; ++j) 
            result(i, j) = primeira(i, j) + segunda(i, j); 

    return result;
}
//
// Para SUBTRACAO, assume que num de linhas e num de colunas sao iguais 
Matriz operator- (Matriz primeira, Matriz segunda) 
{
    if (primeira.numLinhas() != segunda.numLinhas() 
        || primeira.numColunas() != segunda.numColunas())
    {
         throw ErroMatriz{"Matrizes incompatíveis com subtração!\n"};
    }

    int l = primeira.numLinhas();
    int c = primeira.numColunas();

    Matriz result (l, c);
    
    for (int i = 0; i < l; ++i)
        for (int j = 0; j < c; ++j)
            result(i, j) = primeira(i, j) - segunda(i, j);

    return result;
}
//
// Para MULTIPLICAO POR ESCALAR, nao tem requisitos 
Matriz operator* (Matriz matriz, double d) 
{
    int l = matriz.numLinhas();
    int c = matriz.numColunas();

    Matriz result (l, c);

    for (int i = 0; i < l; ++i)
        for (int j = 0; j < c; ++j)
            result(i, j) = d * matriz(i, j);

    return result;
}
// 
// Metodo para receber operacao na ordem contraria
Matriz operator* (double d, Matriz matriz) { return matriz * d; }
//
// Para MULTIPLICACAO, assume que 
// num de linhas da primeira e num de colunas da segunda sao iguais
Matriz operator* (Matriz primeira, Matriz segunda) 
{
    if (primeira.numColunas() != segunda.numLinhas()) {
        throw ErroMatriz {"Matrizes incompatíveis com multiplicação!\n"};
    }

    int l = primeira.numLinhas();
    int c = segunda.numColunas();

    Matriz result (l, c);
    
    for (int i = 0; i < l; ++i) 
    {
        for (int j = 0; j < c; ++j)
        {
            result(i, j) = 0;

            for (int k = 0, fim = primeira.numColunas(); k < fim; ++k) {
                result(i, j) += primeira(i, k) * segunda(k, j); }
        }
    }
    return result;
}

#endif