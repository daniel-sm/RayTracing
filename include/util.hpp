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
// Matriz para operações matematicas 
/*
struct Matriz 
{
private:
    double *matriz;
    int lin, col;
public:
    Matriz (int l, int c) : lin{l}, col{c} { matriz = new double[l * c]; }
    ~Matriz () { delete[] matriz; }

    int getLin () { return lin; }
    int getCol () { return col; }

    double posicao (int i, int j) { return matriz[(i * col) + j]; }
};
*/
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
//
// Operacoes com matriz
/*
Matriz operator+ (Matriz m1, Matriz m2) 
{
    if (m1.getLin() != m2.getLin() or m1.getCol() != m2.getCol()) { return; }
    int nLin = m1.getLin();
    int nCol = m2.getCol();
    Matriz r (nLin, nCol);
    
    for (int i = 0; i < nLin; ++i) {
        for (int j = 0; i < nCol; ++j)
        {
            
        }
    }
}
Matriz operator- (Matriz m1, Matriz m2) {}
Matriz operator* (Matriz m1, Matriz m2) {}
*/


// ********** Funcoes Auxiliares ***********
//
// Produto escalar (interno) entre vetores
double escalar (Vetor v, Vetor u) { return (v.a * u.a) + (v.b * u.b) + (v.c * u.c); }

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

#endif