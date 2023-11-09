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
struct Vetor { double x, y, z; };
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

// ********** Funcoes Auxiliares ***********
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