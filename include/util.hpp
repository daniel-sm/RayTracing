#ifndef UTIL_HPP
#define UTIL_HPP

#include <cmath>

// ********** Tipos de dados *************
//
// Cor com componentes red, green e blue
struct Cor { int r, g, b; };
//
// Coordenadas tridimensionais de um ponto
struct Ponto { double x, y, z; };
//
// Valores de um vetor tridimensional
struct Vetor { double a, b, c; };
//
// Propriedades de cada material 
struct Material 
{
	Vetor ka, kd, ke; // Propriedade de reflectividade
	double brilho; // Fator especular
};

// ********* Operacoes ************
// 
// Operacoes com vetor
Vetor operator+(Vetor v, Vetor u) { return { (v.a + u.a), (v.b + u.b), (v.c + u.c) }; }
Vetor operator-(Vetor v, Vetor u) { return { (v.a - u.a), (v.b - u.b), (v.c - u.c) }; }
Vetor operator*(Vetor v, Vetor u) { return { (v.a * u.a), (v.b * u.b), (v.c * u.c) }; }
//
// Operacoes entre vetor e numero
Vetor operator*(Vetor v, double k) { return { (k * v.a), (k * v.b), (k * v.c) }; }
Vetor operator*(double k, Vetor v) { return { (k * v.a), (k * v.b), (k * v.c) }; }
Vetor operator/(Vetor v, double k) { return { (v.a / k), (v.b / k), (v.c / k) }; }
//
// Operacoes com ponto 
Ponto operator+ (Ponto p, Vetor v) { return { (p.x + v.a), (p.y + v.b), (p.z + v.c) }; }
Vetor operator- (Ponto p1, Ponto p2) { return { (p1.x - p2.x), (p1.y - p2.y), (p1.z - p2.z) }; }

// ********** Funcoes Auxiliares ***********
//
// Produto escalar entre vetores
double escalar (Vetor v, Vetor u) { return (v.a * u.a) + (v.b * u.b) + (v.c * u.c); }

// Produto vetorial entre dois vetores
Vetor vetorial (Vetor v, Vetor u) 
{
    return {
        (v.b * u.c) - (v.c * u.b),
        (v.c * u.a) - (v.a * u.c),
        (v.a * u.b) - (v.b * u.a)
    };
} 

// Modulo (tamanho) de um vetor
double norma(Vetor v) { return sqrt(escalar(v, v)); }

// Calcula o vetor unitario (norma = 1) de m vetor
Vetor unitario (Vetor v) { return (v / norma(v)); }

// Retorna o maior de dois numeros
double maior (double a, double b) { return (a >= b ? a : b); }

// Estrutura de Dados
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

// Classe de Matriz para operacoes matematicas 
//
class Matriz 
{
private:
    double *matriz;
    int linhas, colunas;
public:
    Matriz (int l, int c) : linhas{l}, colunas{c} { matriz = new double[l * c]; }
    ~Matriz () { delete[] matriz; }

    int qtdLinhas () { return linhas; }
    int qtdColunas () { return colunas; }

    double &operator() (int l, int c) { return matriz[(l * colunas) + c]; }

}; // fim class Matriz

// Operacoes com matrizes
// Para as operacoes assume-se que atende os requisitos de cada operacao
//
// Para SOMA, assume que qtd de linhas e qtd de colunas sao iguais 
Matriz operator+ (Matriz& primeira, Matriz& segunda) 
{
    int l = primeira.qtdLinhas();
    int c = primeira.qtdColunas();

    Matriz R(l, c);
    
    for (int i = 0; i < l; ++i) 
    {
        for (int j = 0; j < c; ++j)
        {
            R(i, j) = primeira(i, j) + segunda(i, j);
        }
    }
    return R;
}
//
// Para SUBTRACAO, assume que qtd de linhas e qtd de colunas sao iguais 
Matriz operator- (Matriz& primeira, Matriz& segunda) 
{
    int l = primeira.qtdLinhas();
    int c = primeira.qtdColunas();

    Matriz R(l, c);
    
    for (int i = 0; i < l; ++i) 
    {
        for (int j = 0; j < c; ++j)
        {
            R(i, j) = primeira(i, j) - segunda(i, j);
        }
    }
    return R;
}
//
// Para MULTIPLICAO POR ESCALAR, nao tem requisitos 
Matriz operator* (Matriz& matriz, double d) 
{
    int l = matriz.qtdLinhas();
    int c = matriz.qtdColunas();

    Matriz R(l, c);

    for (int i = 0; i < l; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            R(i, j) *= d;
        }
    }
    return R;
}
// 
// Metodo para receber operacao na ordem contraria
Matriz operator* (double d, Matriz& matriz) { return matriz * d; }
//
// Para MULTIPLICACAO, assume que (qtd de linhas da primeira == qtd de colunas da segunda) 
Matriz operator* (Matriz& primeira, Matriz& segunda) 
{
    int l = primeira.qtdLinhas();
    int c = segunda.qtdColunas();

    Matriz R(l, c);
    
    for (int i = 0; i < l; ++i) 
    {
        for (int j = 0; j < c; ++j)
        {
            R(i, j) = 0;
            
            for (int k = 0, fim = primeira.qtdColunas(); k < fim; ++k) 
            {
                R(i, j) += primeira(i, k) * segunda(k, j);
            }
        }
    }
    return R;
}

#endif