#ifndef BASICS_HPP
#define BASICS_HPP

#include <cmath>
#include <vector>

// Tipos de dados **************************************************************
//
// Cor com componentes red, green e blue
struct Cor { int r, g, b; };
//
// Coordenadas tridimensionais de um ponto
struct Ponto { double x, y, z; };
//
// Valores de um vetor tridimensional
struct Vetor { double x, y, z; };
//
// Propriedades de cada material 
struct Material 
{
	Vetor ka, kd, ke; // Propriedade de reflectividade
	double brilho; // Fator especular
};

// Operacoes *******************************************************************
// 
// Operacoes com vetor
Vetor operator+(Vetor v, Vetor u) { return { (v.x + u.x), (v.y + u.y), (v.z + u.z) }; }
Vetor operator-(Vetor v, Vetor u) { return { (v.x - u.x), (v.y - u.y), (v.z - u.z) }; }
Vetor operator*(Vetor v, Vetor u) { return { (v.x * u.x), (v.y * u.y), (v.z * u.z) }; }
//
// Operacoes entre vetor e numero
Vetor operator*(Vetor v, double k) { return { (k * v.x), (k * v.y), (k * v.z) }; }
Vetor operator*(double k, Vetor v) { return (v * k); }
Vetor operator/(Vetor v, double k) { return { (v.x / k), (v.y / k), (v.z / k) }; }
//
// Operacoes com ponto 
Ponto operator+ (Ponto p, Vetor v) { return { (p.x + v.x), (p.y + v.y), (p.z + v.z) }; }
Vetor operator- (Ponto p1, Ponto p2) { return { (p1.x - p2.x), (p1.y - p2.y), (p1.z - p2.z) }; }

// Funcoes Auxiliares **********************************************************
//
// Produto vetorial entre dois vetores
Vetor vetorial (Vetor v, Vetor u) 
{
    return {
        (v.y * u.z) - (v.z * u.y),
        (v.z * u.x) - (v.x * u.z),
        (v.x * u.y) - (v.y * u.x)
    };
}

// Produto escalar entre vetores
double escalar (Vetor v, Vetor u) { return (v.x * u.x) + (v.y * u.y) + (v.z * u.z); }

// Modulo (tamanho) de um vetor
double norma(Vetor v) { return sqrt(escalar(v, v)); }

// Calcula o vetor unitario (norma = 1) de m vetor
Vetor unitario (Vetor v) { return (v / norma(v)); }

// Retorna o maior de dois numeros
double maior (double a, double b) { return (a >= b ? a : b); }

// Estrutura de Dados **********************************************************
//
// Modelo de classe Lista para guardar os objetos da cena
template <typename T>
class Lista
{
private:
    struct Node {
        T* info;
        Node* prox;
        Node (T* i, Node* p) : info{i}, prox{p} {}
    };
	Node* primeiro;

public:

    class Iterador
    {
    public:
        Node* node;
        Iterador (Node* n) : node{n} {}

        bool operator!= (Iterador j) { return (node != j.node); }
        Iterador& operator++ () { node = node->prox; return *this; }
        T* operator* () { return node->info; }

    }; // fim class Iterador()

	Iterador begin () { return { primeiro }; }
    Iterador end () { return { nullptr }; }

	Lista () : primeiro{nullptr} {}
	~Lista () {
        while (primeiro != nullptr)
        {
            Node *aux = primeiro->prox;
            delete primeiro;
            primeiro = aux;
        }
    }

	Iterador add(T* info) 
	{
        Node *n = new Node(info, primeiro);
		primeiro = n;
        return { n };
	}
}; // fim class Lista

// Classe de Matriz para operacoes matematicas *********************************
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

// Operacoes com Matrizes ******************************************************

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