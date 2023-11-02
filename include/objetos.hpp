#ifndef OBJETOS_HPP
#define OBJETOS_HPP

#include "util.hpp"
#include "matriz.hpp"

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
private:
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
			double t_int = - escalar(normal, w) / denominador;
			return t_int;
		}
		else { return -1; }
	}

    Vetor obterNormal (Ponto p) const override { return normal; }
}; // fim class Plano

class Cilindro : public Objeto 
{
private:
    Ponto base; // centro da base do Cilindro
    Ponto topo; // centro do topo do Cilindro
    Vetor direcao; // direcao do eixo do Cilindro
    double raio; // raio do Cilindro
    double altura; // altura do Cilindro 

public:
    Cilindro (Ponto b, Ponto t, double r, Material m) : base{b}, topo{t}, raio{r}
    {
        direcao = (t - b);
        altura = norma(direcao);
        direcao = direcao / altura;
        material = m;
    }

    Cilindro (Ponto b, Vetor d, double r, double h, Material m) 
    : base{b}, direcao{d}, raio{r}, altura{h}
    {
        topo = b + (h * d);
        material = m;
    }

    double intersecao (Raio r) const override
    {
        // guarda o valor do menor t que intersecta cilindro
        double menor_t = -1;
        // guarda o valor do t para cada intersecao
        double t_int = -1;

        Vetor u = (r.getOrigem() - base);
        Vetor v = u - (escalar(u, direcao) * direcao);
        Vetor w = r.getDirecao() - (escalar(r.getDirecao(), direcao) * direcao);

        double a = escalar(w, w);
        double b = escalar(v, w);
        double c = escalar(v, v) - (raio * raio);

        // a == 0 significa que o raio é paralelo a superficie cilindrica
        if (a != 0)
        {
            double delta = (b * b) - (a * c);

            if (delta >= 0) // significa que tem intersecao
            {
                // duas raizes da equacao (podem ser a mesma raiz)
                double t1 = (-b - sqrt(delta)) / a;
                double t2 = (-b + sqrt(delta)) / a;

                double escalar1 = escalar(r.pontoIntersecao(t1) - base, direcao);
                double escalar2 = escalar(r.pontoIntersecao(t2) - base, direcao);

                // se ponto de t1 for valido, atualiza o valor de t pois é a menor raiz
                if (escalar1 >= 0 && escalar1 <= altura) { t_int = t1; } 
                // se ponto de t2 for valido, atualiza o valor de t pois é a segunda raiz
                else if (escalar2 >= 0 && escalar2 <= altura) { t_int = t2; }
                // se nao houver ponto valido, deixa t com valor invalido
                else { t_int = -1; }
            }
            else { t_int = -1; } // se delta negativo, deixa t com valor invalido
        } else { t_int = -1; } // como a == 0, deixa t com valor invalido

        // atualiza menor_t com o valor de t encontrado
        if(t_int > 0) menor_t = t_int;

        // denominador no calculo da intersecao com plano
		double denominador = escalar(direcao, r.getDirecao());

        // se denominador == 0 nao tem intersecao com plano
		if (denominador != 0)
		{
            // verificando intersecao com base do cilindro
			double t_base = - escalar(direcao, (r.getOrigem() - base)) / denominador;
            // verificando intersecao com topo do cilindro
            double t_topo = - escalar(direcao, (r.getOrigem() - topo)) / denominador;

            Vetor vetorBase = r.pontoIntersecao(t_base) - base;
            Vetor vetorTopo = r.pontoIntersecao(t_topo) - topo;

            double escalarBase = escalar(vetorBase, vetorBase);
            double escalarTopo = escalar(vetorTopo, vetorTopo);

            double quadradoRaio = (raio * raio);
            
            // verficando se sao pontos validos
            if (escalarBase > quadradoRaio) t_base = -1;
            if (escalarTopo > quadradoRaio) t_topo = -1;

            // guarda o menor valor positivo ou -1 
            if (t_base > 0 && t_topo > 0)
            {
                if (t_base < t_topo) t_int = t_base;
                else t_int = t_topo;
            } 
            else if (t_base > 0) t_int = t_base;
            else if (t_topo > 0) t_int = t_topo;
            else t_int = -1;
		}
		else { t_int = -1; }

        // atualiza menor_t com o valor de t encontrado
        if (t_int > 0) 
        {
            if (menor_t > 0) 
                if (t_int < menor_t) 
                    menor_t = t_int; 
            else 
                menor_t = t_int;
        }
    }

    Vetor obterNormal (Ponto p) const override
    {
        Vetor vetorPonto = (p - base);
        double escalarPonto = escalar(vetorPonto, direcao);

        if (escalarPonto == 0) return (-1) * direcao;
        else if (escalarPonto == altura) return direcao;
        else
        {
            Vetor N = vetorPonto - (escalarPonto * direcao);
            return (N / raio);
        }
    }
};

class Cone : public Objeto 
{
    Ponto base; // centro da base do cone
    Ponto vertice; // vertice do cone 
    Vetor direcao; // direcao do eixo do cone
    double raio; // raio da base do cone
    double altura; // altura do cone 

public:
    Cone (Ponto b, Ponto v, double r, Material m) : base{b}, vertice{v}, raio{r}
    {
        direcao = (v - b); 
        altura = norma(direcao);
        direcao = direcao / altura; 
        material = m;
    }

    Cone (Ponto b, Vetor d, double r, double h, Material m) 
    : base{b}, direcao{d}, raio{r}, altura{h} 
    {
        vertice = b + (h * d); 
        material = m;
    }

    double intersecao (Raio r) const override 
    {
        // variavel B das formulas 
        double beta = (altura*altura) / (raio*raio);

        // variavel w das formulas
        Vetor w = r.getOrigem() - base;

        // Produto escalar entre direcao do raio e direcao do eixo do cone
        double escalarRC = escalar(r.getDirecao(), direcao);
        // Produto escalar entre vetor w e direcao do eixo do cone
        double escalarWC = escalar(w, direcao);
        
        Matriz DR (3, 1);
        DR(0, 0) = r.getDirecao().a;
        DR(1, 0) = r.getDirecao().b;
        DR(2, 0) = r.getDirecao().c;

        Matriz DRt = transposta(DR);
        
        Matriz DC (3, 1);
        DC(0, 0) = direcao.a;
        DC(1, 0) = direcao.b;
        DC(2, 0) = direcao.c;

        Matriz DCt = transposta(DC);

        Matriz W (3, 1);
        W(0, 0) = w.a;
        W(1, 0) = w.b;
        W(2, 0) = w.c;

        Matriz Wt = transposta(W);
        
        Matriz P = identidade(3) - (DC * DCt);

        // a = (B * drT * P * dr) - (dr . dc)^2
        // primeiro termo: (B*drT*P*dr)
        // segundo termo: (-(dr.dc)^2)
        double a;
        //  = (beta * DRt) * (P * DR); 

        // b = (2 * B * wT * P * dr) + (2 * H * (dr . dc)) - (2 * (w . dc) * (dr . dc))
        // primeiro termo: (2*B*wT*P*dr) 
        // segundo termo: (2*H*(dr.dc))
        // terceiro termo: (-2*(w.dc)*(dr.dc))
        double b; 

        // c = (B * wT * P * w) - H^2 + (2 * H * (w . dc)) - ((w . dc)^2)
        // primeiro termo: (B*wT*P*w)
        // segundo termo: (-H^2 + 2*H*(w.dc))
        // terceiro termo: (-(w.dc)^2)
        double c; 

        double delta = (b * b) - (4 * a * c);

        if (delta > 0)
        {
            return (-b - sqrt(delta)) / 2*a;
        }
        return -1;
    }

    Vetor obterNormal (Ponto p) const override 
    {
        if (escalar(direcao, (p - base)) == 0)
        {
            return (-1) * direcao;
        } else
        {
            Vetor w = vetorial(direcao, (p - base));
            Vetor normal = vetorial(w, (vertice - p));
            
            return unitario(normal);
        }
    }
}; // fim class Cone
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