#ifndef OBJETOS_HPP
#define OBJETOS_HPP

#include "util.hpp"
#include <iostream>

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

class Objeto
{
public:
    Material material;

	virtual double intersecao (Raio) const = 0;
    virtual Vetor obterNormal (Ponto) const = 0;
};

class Esfera : public Objeto
{
    Ponto centro;
    double raio;
public:
    Esfera(Ponto c, double r, Material m) : centro{c}, raio{r} { material = m; }

    double intersecao (Raio r) const override
    {
        Vetor w = r.origem() - centro;

        double b = escalar( w, r.direcao() );
        double c = escalar( w, w ) - (raio * raio);

        double delta = (b * b) - c;

        if (delta > 0)
        {
            // c diz a distancia da origem do raio ao centro da esfera
            // se c > 0 entao a origem do raio esta fora da esfera
            if (c < 0) { return -b + sqrt(delta); }
            else       { return -b - sqrt(delta); }
        }
        return -1; // se delta <= 0
    }

    Vetor obterNormal (Ponto p) const override { return (p - centro) / raio; }
}; // fim class Esfera

class Plano : public Objeto
{
    Ponto ponto; // ponto pertcente ao plano
    Vetor normal; // vetor normal ao plano
public:
    Plano(Ponto p, Vetor n, Material m) : ponto{p}, normal{unitario(n)} { material = m; }

    double intersecao (Raio raio) const override
	{
		double denominador = escalar(normal, raio.direcao());
		if (denominador != 0)
		{
			Vetor w = raio.origem() - ponto;
			double tInt = - escalar(normal, w) / denominador;
			return tInt;
		}
		else { return -1; }
	}

    Vetor obterNormal (Ponto p) const override { return normal; }
}; // fim class Plano

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

    bool isSombra (
        Ponto p_int, 
        Lista<Objeto> cena,
        double (*raycast)(Lista<Objeto>&, Raio, Objeto*&) 
    ) {
        // Raio que parte da posicao da fonte ao ponto de intersecao
        Raio raioSombra (posicao, p_int);

        // Ponteiro temporario que vai guardar o objeto atingido
        Objeto* temp; // Necessario na funcao mas nao sera usado 

        // Lancando raio na cena para ver se tem sombra
        double t_sombra = raycast(cena, raioSombra, temp);

        // Obtendo distancia da fonte ao ponto de intersecao
        double distanciaFonte = norma(p_int - posicao);

        return (t_sombra + 0.01 >= distanciaFonte);
    }

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