#ifndef OBJETOS_HPP
#define OBJETOS_HPP

#include "basics.hpp"

class Janela
{
private:
    double xmin, xmax, ymin, ymax, distance;
public:
    Janela(double xm, double xM, double ym, double yM, double d) 
    : xmin{xm}, xmax{xM}, ymin{ym}, ymax{yM}, distance{d} {}

    double getXMin() { return xmin; }
    double getYMax() { return ymax; }
    double getWidth() { return xmax - xmin; }
    double getHeight() { return ymax - ymin; }
    double getDistance() { return distance; }
}; // fim class Janela

class Raio 
{
private:
	Ponto origem; // ponto de origem do raio
	Vetor direcao; // vetor unitario da direcao do raio 
public:
	Raio(Ponto i, Ponto f) : origem{i}, direcao{unitario(f - i)} {}
	Raio(Ponto i, Vetor d) : origem{i}, direcao{unitario(d)} {}

	Vetor getDirecao() { return direcao; }
	Ponto getOrigem() { return origem; }

	// Ponto de intersecao com parametro "t" 
	Ponto pontoIntersecao (double t) { return { origem + (t * direcao) }; }
}; // fim class Raio

class Objeto
{
public:
    Material material;
	virtual double intersecao (Raio) = 0;
    virtual Vetor obterNormal (Ponto) const = 0;
    virtual void transformar (Matriz) = 0;
};

// Gerando um tipo chamado RC que correponde ao tipo da funcao raycast
// Esse tipo sera usado para um ponteiro da função raycast
using RC = double(Lista<Objeto>&, Raio, Objeto*&);

class Fonte 
{
public:
	Vetor intensidade; // intensidade da luz

    virtual Vetor iluminacao(Vetor, Ponto, Vetor, Material) const = 0;
    virtual bool sombra(Ponto, Lista<Objeto>&, RC*) const = 0;
    virtual void transformar (Matriz) = 0;
};

class Camera 
{
private:
    Ponto eye, at, up;
    Vetor viewup, i, j, k;
    Matriz w2c, c2w;
public:
    Camera (Ponto e, Ponto a, Ponto u) : eye{e}, at{a}, up{u}, viewup{u - e}
    {
        // eixo Z do sistema de coordenadas da camera
        k = unitario(eye - at);
        // eixo X do sistema de coordenadas da camera
        i = unitario(vetorial(viewup, k));
        // eixo Y do sistema de coordenadas da camera
        j = vetorial(k, i);

        // Definindo da matriz 4x4 de mundo para camera
        w2c.setSize(4, 4);
        // Definindo a primeira linha da matriz
        w2c(0, 0) = i.x; w2c(0, 1) = i.y; w2c(0, 2) = i.z; 
        w2c(0, 3) = (-1) * escalar(i, { eye.x, eye.y, eye.z });
        // Definindo a segunda linha da matriz
        w2c(1, 0) = j.x; w2c(1, 1) = j.y; w2c(1, 2) = j.z; 
        w2c(1, 3) = (-1) * escalar(j, { eye.x, eye.y, eye.z });
        // Definindo a terceira linha da matriz
        w2c(2, 0) = k.x; w2c(2, 1) = k.y; w2c(2, 2) = k.z; 
        w2c(2, 3) = (-1) * escalar(k, { eye.x, eye.y, eye.z });
        // Definindo a quarta linha da matriz
        w2c(3, 0) = 0; w2c(3, 1) = 0; w2c(3, 2) = 0; w2c(3, 3) = 1;

        // Definindo da matriz 4x4 de camera para mundo
        c2w.setSize(4, 4);
        // Definindo a primeira linha da matriz
        c2w(0, 0) = i.x; c2w(0, 1) = j.x; c2w(0, 2) = k.x; c2w(0, 3) = eye.x;
        // Definindo a segunda linha da matriz
        c2w(1, 0) = i.y; c2w(1, 1) = j.y; c2w(1, 2) = k.y; c2w(1, 3) = eye.y;
        // Definindo a terceira linha da matriz
        c2w(2, 0) = i.z; c2w(2, 1) = j.z; c2w(2, 2) = k.z; c2w(2, 3) = eye.z;
        // Definindo a quarta linha da matriz
        c2w(3, 0) = 0; c2w(3, 1) = 0; c2w(3, 2) = 0; c2w(3, 3) = 1;
    }

    void toCamera (Lista<Objeto> &cena, Lista<Fonte> &fontes) 
    {
        // Percorrendo os objetos e transformando em coordenadas de camera
        for (auto obj : cena) { obj->transformar(w2c); }
        // Percorrendo as fontes e transformando em coordenadas de camera
        for (auto fonte : fontes) { fonte->transformar(w2c); }
    }

    void toWorld (Lista<Objeto> &cena, Lista<Fonte> &fontes) 
    {
        // Percorrendo os objetos e transformando em coordenadas de mundo
        for (auto obj : cena) { obj->transformar(c2w); }
        // Percorrendo as fontes e transformando em coordenadas de mundo
        for (auto fonte : fontes) { fonte->transformar(c2w); };
    }
};

// Hierarquia de classes: Objeto
//
class Esfera : public Objeto
{
private:
    Ponto centro;
    double raio;

public:
    Esfera(Ponto c, double r, Material m) : centro{c}, raio{r} { material = m; }

    double intersecao (Raio r) override;

    Vetor obterNormal (Ponto p) const override { return (p - centro) / raio; }

    void transformar (Matriz matriz) override
    {
        // Definindo ponto em formato de matriz para fazer transformacao
        Matriz p (4, 1);
        p(0, 0) = centro.x; p(1, 0) = centro.y; p(2, 0) = centro.z; p(3, 0) = 1;
        // Realizando multiplicacao
        p = matriz * p; // transformando o ponto do centro da esfera
        // Definindo novas coordenadas do centro da esfera
        centro.x = p(0, 0); centro.y = p(1, 0); centro.z = p(2, 0);
    }
}; // fim class Esfera

class Plano : public Objeto
{
    Ponto ponto; // ponto pertencente ao plano
    Vetor normal; // vetor normal ao plano
public:
    Plano(Ponto p, Vetor n, Material m) : ponto{p}, normal{unitario(n)} { material = m; }

    double intersecao (Raio raio) override;

    Vetor obterNormal (Ponto p) const override { return normal; }

    void transformar (Matriz matriz) override
    {
        // Definindo ponto em formato de matriz para fazer transformacao
        Matriz p (4, 1);
        p(0, 0) = ponto.x; p(1, 0) = ponto.y; p(2, 0) = ponto.z; p(3, 0) = 1;
        // Definindo vetor em formato de matriz para fazer transformacao
        Matriz v (4, 1);
        v(0, 0) = normal.x; v(1, 0) = normal.y; v(2, 0) = normal.z; v(3, 0) = 0;
        // Realizando multiplicacoes
        p = matriz * p; // transformando ponto do plano
        v = matriz * v; // transformando vetor normal ao plano
        // Definindo novas coordenadas do ponto do plano
        ponto.x = p(0, 0); ponto.y = p(1, 0); ponto.z = p(2, 0);
        // Definindo novas coordenadas do vetor normal ao plano
        normal.x = v(0, 0); normal.y = v(1, 0); normal.z = v(2, 0);
        // Ajustando vetor normal ao plano para unitario
        normal = unitario(normal);
    }
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
    : base{b}, direcao{unitario(d)}, raio{r}, altura{h}
    {
        topo = b + (h * direcao);
        material = m;
    } 

    double intersecao (Raio r) override;

    Vetor obterNormal (Ponto p) const override
    {
        Vetor vetorPonto = (p - base);
        double escalarPonto = escalar(vetorPonto, direcao);

        if (escalarPonto - 0.1 <= 0) return (-1) * direcao;
        else if (escalarPonto + 0.1 >= altura) return direcao;
        else
        {
            Vetor N = vetorPonto - (escalarPonto * direcao);
            return (N / raio);
        }
    }

    void transformar (Matriz matriz) override
    {
        // Definindo ponto da base em formato de matriz para fazer transformacao
        Matriz b (4, 1);
        b(0, 0) = base.x; b(1, 0) = base.y; b(2, 0) = base.z; b(3, 0) = 1;
        // Definindo ponto do topo em formato de matriz para fazer transformacao
        Matriz t (4, 1);
        t(0, 0) = topo.x; t(1, 0) = topo.y; t(2, 0) = topo.z; t(3, 0) = 1;
        // Definindo vetor direcao em formato de matriz para fazer transformacao
        Matriz d (4, 1);
        d(0, 0) = direcao.x; d(1, 0) = direcao.y; d(2, 0) = direcao.z; d(3, 0) = 0;
        // Realizando multiplicacoes
        b = matriz * b; // transformando ponto do centro da base
        t = matriz * t; // transformando ponto do centro do topo
        d = matriz * d; // transformando vetor direcao do cilindro
        // Definindo novas coordenadas do ponto do centro da base
        base.x = b(0, 0); base.y = b(1, 0); base.z = b(2, 0);
        // Definindo novas coordenadas do ponto do centro do topo
        topo.x = t(0, 0); topo.y = t(1, 0); topo.z = t(2, 0);
        // Definindo novas coordenadas do vetor direcao do cilindro
        direcao.x = d(0, 0); direcao.y = d(1, 0); direcao.z = d(2, 0);
        // Ajustando vetor direcao do cilindro para unitario
        direcao = unitario(direcao);
    }
}; // fim class Cilindro

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
    : base{b}, direcao{unitario(d)}, raio{r}, altura{h} 
    {
        vertice = b + (h * direcao); 
        material = m;
    }

    double intersecao (Raio r) override;

    Vetor obterNormal (Ponto p) const override 
    {
        if (escalar(direcao, (p - base)) - 0.1 <= 0) { return (-1) * direcao; }
        else
        {
            Vetor w = vetorial(direcao, (p - base));
            Vetor normal = vetorial(w, (vertice - p));
            
            return unitario(normal);
        }
    }

    void transformar (Matriz matriz) override
    {
        // Definindo ponto da base em formato de matriz para fazer transformacao
        Matriz b (4, 1);
        b(0, 0) = base.x; b(1, 0) = base.y; b(2, 0) = base.z; b(3, 0) = 1;
        // Definindo ponto do vertice em formato de matriz para fazer transformacao
        Matriz v (4, 1);
        v(0, 0) = vertice.x; v(1, 0) = vertice.y; v(2, 0) = vertice.z; v(3, 0) = 1;
        // Definindo vetor direcao em formato de matriz para fazer transformacao
        Matriz d (4, 1);
        d(0, 0) = direcao.x; d(1, 0) = direcao.y; d(2, 0) = direcao.z; d(3, 0) = 0;
        // Realizando multiplicacoes
        b = matriz * b; // transformando ponto do centro da base
        v = matriz * v; // transformando ponto do centro do vertice
        d = matriz * d; // transformando vetor direcao do cone
        // Definindo novas coordenadas do ponto do centro da base
        base.x = b(0, 0); base.y = b(1, 0); base.z = b(2, 0);
        // Definindo novas coordenadas do ponto do centro do vertice
        vertice.x = v(0, 0); vertice.y = v(1, 0); vertice.z = v(2, 0);
        // Definindo novas coordenadas do vetor direcao do cone
        direcao.x = d(0, 0); direcao.y = d(1, 0); direcao.z = d(2, 0);
        // Ajustando vetor direcao do cone para unitario
        direcao = unitario(direcao);
    }
}; // fim class Cone

class Malha : public Objeto
{
private:
    // estrutura de Aresta para a lista de arestas da malha
    struct Aresta { int v1, v2; };
    // estrutura de Face para a lista de faces da malha
    struct Face { int a1, a2, a3; };

    // quantidade de vertices, arestas e faces na malha
    int numvertices, numarestas, numfaces;

    // std::vector<Ponto> vertices; // lista de vertices
    // std::vector<Aresta> arestas; // lista de arestas
    // std::vector<Face> faces; // lista de faces
    Ponto* vertices; // lista de vertices
    Aresta* arestas; // lista de arestas
    Face* faces; // lista de faces

    // variavel que guarda o id da face atingida pelo raio
    // apos calcular intersecao atualiza a variavel
    // quando pedir a normal de um ponto, usa a face atingida 
    int face_atingida = -1;

public:
    Malha(int v, int a, int f, Material m) : numvertices{v}, numarestas{a}, numfaces{f}
    {
        // vertices.resize(v);
        // arestas.resize(a);
        // faces.resize(f);
        vertices = new Ponto[v];
        arestas = new Aresta[a];
        faces = new Face[f];
        material = m;
    }
    ~Malha () 
    { delete[] vertices; delete[] arestas; delete[] faces; }

    // recebe id do vertice e posicao do vertice
    void setVertice (int i, Ponto p) { vertices[i] = p; }
    // recebe id da aresta e ids dos vertices
    void setAresta (int i, int v1, int v2) { arestas[i] = { v1, v2 }; }
    // recebe id da face e ids das arestas
    void setFace (int i, int a1, int a2, int a3) { faces[i] = { a1, a2, a3 }; }

    double intersecao (Raio raio) override;

    Vetor obterNormal (Ponto ponto) const override 
    {
        // obtendo id das arestas da face que foi atingida por ultimo
        int a1 = faces[face_atingida].a1;
        int a2 = faces[face_atingida].a2;

        // id dos vertices 1 e 2 da aresta 1, somando +1 para evitar id = 0
        int v1_a1 = arestas[a1].v1 + 1; 
        int v2_a1 = arestas[a1].v2 + 1; 

        // id dos vertices 1 e 2 da aresta 2, somando +1 para evitar id = 0
        int v1_a2 = arestas[a2].v1 + 1; 
        int v2_a2 = arestas[a2].v2 + 1; 

        int v1, v2, v3;

        // algoritmo para encontrar id do vertice comum 
        // e id dos vertices na ordem anti-horaria 
        float n1 = v1_a1 * v2_a1;
        float n = n1 / v1_a2;

        if (n == v1_a1 || n == v2_a1)
        {
            v1 = v1_a2;
            v2 = v2_a2;
            v3 = n;
        } else 
        {
            v1 = v2_a2;
            v2 = v1_a2;
            v3 = n1/v1;
        } 
        // reajustando o valor dos ids para o original
        --v1; --v2; --v3;
        // apos if e else, v1 tem o id do vertice comum
        // v2 e v3 tem o id dos vertices vizinhos na ordem anti-horaria

        // definindo p1, p2 e p3 como os pontos dos ids v1, v2 e v3
        Ponto p1 = vertices[v1]; 
        Ponto p2 = vertices[v2]; 
        Ponto p3 = vertices[v3]; 

        // definindo os vetores que partem do vertice comum v1 
        Vetor r1 = p2 - p1;
        Vetor r2 = p3 - p1;

        // definindo vetor normal nao normalizado
        Vetor N = vetorial(r1, r2); 

        // vetor normal unitario 
        Vetor normal = N / norma(N);

        return normal;
    }

    void transformar (Matriz matriz) override
    {
        for (int i = 0; i < numvertices; ++i)
        {
            // Definindo ponto do vertice atual
            Ponto v = vertices[i];
            // Definindo ponto da base em formato de matriz para fazer transformacao
            Matriz p (4, 1);
            p(0, 0) = v.x; p(1, 0) = v.y; p(2, 0) = v.z; p(3, 0) = 1;
            // Realizando multiplicacoes
            p = matriz * p; // transformando ponto do vertice atual
            // Definindo novas coordenadas do ponto do vertice atual
            vertices[i].x = p(0, 0); vertices[i].y = p(1, 0); vertices[i].z = p(2, 0);
        }
    }

    void copiar (Malha* outro) 
    { // Assume que a nova malha tem mesmo numero de vertices, arestas e faces
        // outro->vertices = vertices;
        // outro->arestas = arestas;
        // outro->faces = faces;
        for (int i = 0; i < numvertices; ++i) outro->vertices[i] = vertices[i];
        for (int i = 0; i < numarestas; ++i) outro->arestas[i] = arestas[i];
        for (int i = 0; i < numfaces; ++i) outro->faces[i] = faces[i];
    }

    // Funcao que inverte a ordem das arestas das faces quando se usa espelho
    void inverterOrdem () 
    {
        for (int i = 0; i < numfaces; ++i)
        {
            int aux = faces[i].a1;
            faces[i].a1 = faces[i].a3;
            faces[i].a3 = aux;
        }
    }
}; // fim class Malha
//
// Fim da Hierarquia de classes Objeto 

// Hierarquia de classes: Fonte
//
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

    void transformar (Matriz matriz) override
    {
        // Definindo ponto da fonte em formato de matriz para fazer transformacao
        Matriz p (4, 1);
        p(0, 0) = posicao.x; p(1, 0) = posicao.y; p(2, 0) = posicao.z; p(3, 0) = 1;
        // Realizando multiplicacao
        p = matriz * p; // transformando ponto da fonte
        // Definindo novas coordenadas do ponto da fonte
        posicao.x = p(0, 0); posicao.y = p(1, 0); posicao.z = p(2, 0);
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

    void transformar (Matriz matriz) override
    {
        // Definindo ponto da fonte em formato de matriz para fazer transformacao
        Matriz p (4, 1);
        p(0, 0) = posicao.x; p(1, 0) = posicao.y; p(2, 0) = posicao.z; p(3, 0) = 1;
        // Definindo vetor direcao em formato de matriz para fazer transformacao
        Matriz d (4, 1);
        d(0, 0) = direcao.x; d(1, 0) = direcao.y; d(2, 0) = direcao.z; d(3, 0) = 0;
        // Realizando multiplicacoes
        p = matriz * p; // transformando ponto da fonte
        d = matriz * d; // transformando vetor direcao da fonte
        // Definindo novas coordenadas do ponto da fonte
        posicao.x = p(0, 0); posicao.y = p(1, 0); posicao.z = p(2, 0);
        // Definindo novas coordenadas do vetor direcao da fonte
        direcao.x = d(0, 0); direcao.y = d(1, 0); direcao.z = d(2, 0);
        // Ajustando vetor direcao da fonte para unitario
        direcao = unitario(direcao);
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
        Raio raioSombra (inicio, (-1) * direcao);

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

    void transformar (Matriz matriz) override
    {
        // Definindo vetor direcao em formato de matriz para fazer transformacao
        Matriz d (4, 1);
        d(0, 0) = direcao.x; d(1, 0) = direcao.y; d(2, 0) = direcao.z; d(3, 0) = 0;
        // Realizando multiplicacoes
        d = matriz * d; // transformando vetor direcao da fonte
        // Definindo novas coordenadas do vetor direcao da fonte
        direcao.x = d(0, 0); direcao.y = d(1, 0); direcao.z = d(2, 0);
        // Ajustando vetor direcao da fonte para unitario
        direcao = unitario(direcao);
    }
}; // fim class Direcional
//
// Fim da hierarquia de classes Fonte
#endif