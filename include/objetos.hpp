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
	Raio(Ponto i, Ponto f) : ini{i}, dir{unitario(f - i)} {}
	Raio(Vetor d, Ponto i) : ini{i}, dir{unitario(d)} {}

	Vetor getDirecao() { return dir; }
	Ponto getOrigem() { return ini; }

	// Ponto de intersecao com parametro "t" 
	Ponto pontoIntersecao (double t) { return { ini + (t * dir) }; }
}; // fim class Raio

// Hierarquia de classes: Objeto
//
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
        Vetor w = r.getOrigem() - centro;

        double b = escalar( w, r.getDirecao() );
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
    Plano(Ponto p, Vetor n, Material m) : ponto{p}, normal{unitario(n)} 
    {
        material = m; 
    }

    double intersecao (Raio raio) const override
	{
		double denominador = escalar(normal, raio.getDirecao());
		if (denominador != 0)
		{
			Vetor w = raio.getOrigem() - ponto;
			double tInt = - escalar(normal, w) / denominador;
			return tInt;
		}
		else { return -1; }
	}

    Vetor obterNormal (Ponto p) const override { return normal; }
}; // fim class Plano

class Cone : public Objeto 
{
    Ponto centro; // centro da case do cone
    Ponto vertice; // vertice do cone 
    Vetor direcao; // direcao do do eixo do cone
    double raio; // raio da base do cone
    double altura; // altura do cone 

public:
    Cone (Ponto c, Vetor d, double r, double h, Material m) 
    : centro{c}, direcao{d}, raio{r}, altura{h} 
    {
        vertice = centro + (altura * direcao); 
        material = m;
    }

    Cone (Ponto c, Ponto v, double r, Material m) 
    : centro{c}, vertice{v}, raio{r}
    {
        altura = norma(v - c);
        direcao = (v - c) / altura; 
        material = m;
    }

    double intersecao (Raio r) const override 
    {
        // variavel B das formulas 
        double beta = (altura*altura) / (raio*raio);

        // variavel w das formulas
        Vetor w = r.getOrigem() - centro;

        // Produto escalar entre direcao do raio e direcao do eixo do cone
        double escalarRC = escalar(r.getDirecao(), direcao);
        // Produto escalar entre vetor w e direcao do eixo do cone
        double escalarWC = escalar(w, direcao);
        
        // coordenadas do vetor direcao
        double xd = r.getDirecao().a;
        double yd = r.getDirecao().b;
        double zd = r.getDirecao().c;
        
        // coordenadas do vetor w
        double xw = w.a;
        double yw = w.b;
        double zw = w.c;

        // a = (B.drT.P.dr) - (dr.dc)^2
        double a; 
        // primeiro termo: (B*drT*P*dr)
        a = (xd*xd) + (yd*yd) + (zd*zd) - (xd*xd*xd*xd) - (yd*yd*yd*yd) - (zd*zd*zd*zd);
        a = a - 2*(xd*xd*yd*yd) - 2*(xd*xd*zd*zd) - 2*(yd*yd*zd*zd);
        a = a * beta;
        // segundo termo: (-(dr.dc)^2)
        a = a - escalarRC * escalarRC;

        // b = 2*B*wT*P*dr + 2*H*(dr.dc) - 2*(w.dc)*(dr.dc)
        double b; 
        // primeiro termo: (2*B*wT*P*dr) 
        b = (xw*xd) + (yw*yd) + (zw*zd) - (xw*xd*xd*xd) - (yw*yd*yd*yd) - (zw*zd*zd*zd);
        b = b - xw*xd*(yd*yd + zd*zd) - yw*yd*(xd*xd + zd*zd) - zw*zd*(xd*xd + yd*yd);
        b = b * 2 * beta;
        // segundo termo: (2*H*(dr.dc))
        b = b + 2 * altura * escalarRC;
        // terceiro termo: (-2*(w.dc)*(dr.dc))
        b = b - 2 * escalarWC * escalarRC;

        // c = B*wT*P*w - H^2 + 2*H*(w.dc) - (w.dc)^2
        double c; 
        // primeiro termo: (B*wT*P*w)
        c = (xw*xw) + (yw*yw) + (zw*zw) - (xw*xw*xw*xw) - (yw*yw*yw*yw) - (zw*zw*zw*zw);
        c = c - 2*(xw*xw*yw*yw) - 2*(xw*xw*zw*zw) - 2*(yw*yw*zw*zw);
        c = c * beta;
        // segundo termo: (-H^2 + 2*H*(w.dc))
        c = c - altura*altura + 2 * altura * escalarWC;
        // terceiro termo: (-(w.dc)^2)
        c = c - escalarWC * escalarWC;

        double delta = (b * b) - (4 * a * c);

        if (delta > 0)
        {
            return (-b - sqrt(delta)) / 2*a;
        }
        return -1;
    }

    Vetor obterNormal (Ponto p) const override 
    {
        if (escalar(direcao, (p - centro)) == 0)
        {
            return (-1) * direcao;
        } else
        {
            Vetor w = vetorial(direcao, (p - centro));
            Vetor normal = vetorial(w, (vertice - p));
            return unitario(normal);
        }
        
        
    }
};
//
// Fim da hierarquia de classes Objeto 

// Gerando um tipo chamado RC que correponde ao tipo da funcao raycast
// Esse tipo sera usado para um ponteiro da função raycast
using RC = double(Lista<Objeto>&, Raio, Objeto*&);

// Hierarquia de classes: Fonte
//
class Fonte 
{
public:
	Vetor intensidade; // intensidade da luz

    virtual Vetor iluminacao(Vetor, Ponto, Vetor, Material) const = 0;
    virtual bool sombra(Ponto, Lista<Objeto>&, RC*) const = 0;
};

class Pontual : public Fonte
{
    Ponto posicao; // local da fonte de luz
public:
    Pontual (Vetor i, Ponto p) : posicao{p} { intensidade = i; }

    bool sombra (Ponto p_int, Lista<Objeto> &cena, RC* raycast) const override 
    {
        // Raio que parte da posicao da fonte ao ponto de intersecao
        Raio raioSombra (posicao, p_int);

        // Ponteiro temporario que vai guardar o objeto atingido
        Objeto* temp; // Necessario na funcao mas nao sera usado 

        // Lancando raio na cena para ver se tem sombra
        double t_sombra = raycast(cena, raioSombra, temp);

        // Obtendo distancia da fonte ao ponto de intersecao
        double distanciaFonte = norma(p_int - posicao);

        // Retorna TRUE se distancia da intersecao for igual ou maior
        return (t_sombra + 0.01 >= distanciaFonte);
    }

    Vetor iluminacao (Vetor normal, Ponto p_int, Vetor dirRaio, Material material) 
    const override
	{
        // Vetor em direcao a fonte de luz  (unitario)
        Vetor luz = unitario(posicao - p_int);
        // Vetor em direcao a origem do raio (unitario)
        Vetor visao = (-1) * dirRaio;
        // Vetor em direcao ao raio refletido (unitario)
        Vetor reflexo = (2 * escalar(luz, normal) * normal) - luz;

        // Fator de Difusao
        double fatorDif = maior(0.0, escalar(luz, normal));
        // Calculo da intensidade da Luz Difusa
        Vetor Id = (material.kd * intensidade) * fatorDif; 

        // Fator Especular
        double fatorEsp = maior(0.0, escalar(reflexo, visao));
        fatorEsp = pow(fatorEsp, material.brilho);
        // Calculo da intensidade da Luz Especular 
        Vetor Ie = (material.ke * intensidade) * fatorEsp; 

        // Retorna a soma das duas intensidades
        return (Id + Ie);
	}
}; // fim class Pontual

class Spot : public Fonte
{
private:
    Ponto posicao; // local da fonte de luz
    Vetor direcao; // direcao da fonte de luz
    double angulo; // angulo de abertura da fonte de luz
public:
    Spot(Vetor i, Ponto p, Vetor d, double a) : 
        posicao{p}, 
        direcao{unitario(d)}, 
        angulo{a} 
        { intensidade = i; }

    bool sombra (Ponto p_int, Lista<Objeto> &cena, RC* raycast) const override
    {
        // Vetor em direcao a fonte de luz  (unitario)
        Vetor luz = unitario(posicao - p_int);
        // Cosseno entre o vetor luz e a direcao da luz spot
        double cosLuzDirecao = escalar((-1 * luz), direcao);

        // Verificando se o ponto intersectado nao esta no foco da luz 
        if (cos(angulo) > cosLuzDirecao) return false;

        // Raio que parte da posicao da fonte ao ponto de intersecao
        Raio raioSombra (posicao, p_int);

        // Ponteiro temporario que vai guardar o objeto atingido
        Objeto* temp; // Necessario na funcao mas nao sera usado 

        // Lancando raio na cena para ver se tem sombra
        double t_sombra = raycast(cena, raioSombra, temp);

        // Obtendo distancia da fonte ao ponto de intersecao
        double distanciaFonte = norma(p_int - posicao);

        // Retorna TRUE se distancia da intersecao for igual ou maior
        return (t_sombra + 0.01 >= distanciaFonte);
    }

    Vetor iluminacao (Vetor normal, Ponto p_int, Vetor dirRaio, Material material)
    const override
    {
        // Vetor em direcao a fonte de luz  (unitario)
        Vetor luz = unitario(posicao - p_int);
        // Vetor em direcao a origem do raio (unitario)
        Vetor visao = (-1) * dirRaio;
        // Vetor em direcao ao raio refletido (unitario)
        Vetor reflexo = (2 * escalar(luz, normal) * normal) - luz;

        // Fator de Difusao e Especular
        double fatorDif, fatorEsp;

        // Cosseno entre o vetor luz e a direcao da luz spot
        double cosLuzDirecao = escalar((-1 * luz), direcao);

        if (cos(angulo) <= cosLuzDirecao)
        {
            // Fator de Difusao
            fatorDif = maior(0.0, escalar(luz, normal));
            // Fator Especular
            fatorEsp = maior(0.0, escalar(reflexo, visao));
            fatorEsp = pow(fatorEsp, material.brilho);
        } else
        {
            fatorDif = 0; // Fator de Difusao
            fatorEsp = 0; // Fator Especular
        }
        
        // Calculo da intensidade da Luz Difusa
        Vetor Id = (material.kd * intensidade) * fatorDif; 
        // Calculo da intensidade da Luz Especular 
        Vetor Ie = (material.ke * intensidade) * fatorEsp; 
        
        return (Id + Ie);// * cosLuzDirecao;
    }
}; // fim class Spot

class Direcional : public Fonte
{
private:
    Vetor direcao;
public:
    Direcional(Vetor i, Vetor d) : direcao{unitario(d)} { intensidade = i; }

    bool sombra (Ponto p_int, Lista<Objeto> &cena, RC* raycast) const override
    {
        // Raio que parte do ponto de intersecao em direcao a fonte
        // Como nao tem posicao da fonte entao gera-se um ponto qualquer 
        // na direcao contraria da direcao da fonte de luz direcional
        Ponto inicio = p_int + (-0.1 * direcao);
        Raio raioSombra ((-1) * direcao, inicio);

        // Ponteiro temporario que vai guardar o objeto atingido
        Objeto* temp; // Necessario na funcao mas nao sera usado 

        // Lancando raio na cena para ver se tem sombra
        double t_sombra = raycast(cena, raioSombra, temp);

        // Retorna TRUE se houve qualquer intersecao no caminho
        // Como nao tem posicao, qualquer intersecao torna-se valida
        return (t_sombra <= 0);
    }

    Vetor iluminacao (Vetor normal, Ponto p_int, Vetor dirRaio, Material material)
    const override
    {
        // Vetor em direcao a fonte de luz  (unitario)
        Vetor luz = (-1) * direcao;
        // Vetor em direcao a origem do raio (unitario)
        Vetor visao = (-1) * dirRaio;
        // Vetor em direcao ao raio refletido (unitario)
        Vetor reflexo = (2 * escalar(luz, normal) * normal) - luz;

        // Fator de Difusao
        double fatorDif = maior(0.0, escalar(luz, normal));
        // Calculo da intensidade da Luz Difusa
        Vetor Id = (material.kd * intensidade) * fatorDif; 

        // Fator Especular
        double fatorEsp = maior(0.0, escalar(reflexo, visao));
        fatorEsp = pow(fatorEsp, material.brilho);
        // Calculo da intensidade da Luz Especular 
        Vetor Ie = (material.ke * intensidade) * fatorEsp; 

        // Retorna a soma das duas intensidades
        return (Id + Ie);
    }
}; // fim class Direcional
//
// Fim da hierarquia de classes Fonte
#endif