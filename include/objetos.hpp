#ifndef OBJETOS_HPP
#define OBJETOS_HPP

#include "util.hpp"

class Janela
{
private:
    double width, height, distance;
public:
    Janela(double w, double h, double d) : width{w}, height{h}, distance{d} {}

    double getWidth() { return width; }
    double getHeight() { return height; }
    double getDistance() { return distance; }
}; // fim class Janela

class Raio 
{
private:
	Ponto ini; // origem
	Vetor dir; // unitario
public:
	Raio(Ponto in, Ponto pr) : ini{in}, dir{unitario(pr - in)} {}

	Vetor direcao() { return dir; }
	Ponto origem() { return ini; }

	// Ponto de intersecao com parametro "t" 
	Ponto pontoIntersecao (double t) { return { ini + (t * dir) }; }
}; // fim class Raio

// Hierarquia de classes: Fonte
class Fonte 
{
public:
	Vetor intensidade; // intensidade da luz

    virtual Vetor calcIntensidade(Vetor, Ponto, Vetor, Material) const = 0;
};

class Pontual : public Fonte
{
    Ponto posicao; // local da luz
public:
    Pontual (Vetor i, Ponto p) : posicao{p} { intensidade = i; }

    Ponto getPosicao () { return posicao; }

    Vetor calcIntensidade (Vetor n, Ponto p_int, Vetor dr, Material m) 
    const override
	{
        // Vetor em direcao a fonte de luz  (unitario)
        Vetor l = unitario(posicao - p_int);
        // Vetor em direcao a origem do raio (unitario)
        Vetor v = -1 * dr;
        // Vetor em direcao ao raio refletido (unitario)
        Vetor r = (2 * escalar(l, n) * n) - l;

        // Fator de Difusao
        double fatorDif = maior(0.0, escalar(l, n));
        // Luz Difusa
        Vetor Id = (m.kd * intensidade) * fatorDif; 

        // Fator Especular
        double fatorEsp = pow(maior(0.0, escalar(r, v)), m.brilho);
        // Luz Especular 
        Vetor Ie = (m.ke * intensidade) * fatorEsp; 

        return (Id + Ie);
	}
};

#endif