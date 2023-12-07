#ifndef COMPLEXOS_HPP
#define COMPLEXOS_HPP

#include <SDL.h>
#include "basics.hpp"
#include "objetos.hpp"
#include "transformacoes.hpp"

class Casa : public Objeto
{
private:
    Malha  *parede, *telhado_esq, *telhado_dir, *atingido;

public:
    Casa(Material mp, Material mt)
    {
        Malha cubo (8, 18, 12, {});
        parede = new Malha(8, 18, 12, mp);
        telhado_esq = new Malha(8, 18, 12, mt);
        telhado_dir = new Malha(8, 18, 12, mt);

        // Definindo os vertices do Cubo ***************************************
        cubo.setVertice(0, {0, 0, 1}); cubo.setVertice(1, {1, 0, 1});
        cubo.setVertice(2, {1, 1, 1}); cubo.setVertice(3, {0, 1, 1});
        cubo.setVertice(4, {0, 0, 0}); cubo.setVertice(5, {1, 0, 0});
        cubo.setVertice(6, {1, 1, 0}); cubo.setVertice(7, {0, 1, 0});
        // Definindo as arestas do Cubo
        cubo.setAresta(0, 0, 1); cubo.setAresta(1, 1, 2); cubo.setAresta(2, 2, 3);
        cubo.setAresta(3, 3, 0); cubo.setAresta(4, 0, 4); cubo.setAresta(5, 1, 5);
        cubo.setAresta(6, 2, 6); cubo.setAresta(7, 3, 7); cubo.setAresta(8, 4, 5);
        cubo.setAresta(9, 5, 6); cubo.setAresta(10, 6, 7); cubo.setAresta(11, 7, 4);
        cubo.setAresta(12, 1, 3); cubo.setAresta(13, 2, 5); cubo.setAresta(14, 4, 6);
        cubo.setAresta(15, 0, 7); cubo.setAresta(16, 3, 6); cubo.setAresta(17, 0, 5);
        // Definindo as faces do Cubo
        cubo.setFace(0, 3, 0, 12); cubo.setFace(1, 1, 2, 12); cubo.setFace(2, 1, 5, 13);
        cubo.setFace(3, 9, 6, 13); cubo.setFace(4, 9, 8, 14); cubo.setFace(5, 11, 10, 14);
        cubo.setFace(6, 4, 3, 15); cubo.setFace(7, 7, 11, 15); cubo.setFace(8, 2, 6, 16);
        cubo.setFace(9, 10, 7, 16); cubo.setFace(10, 4, 8, 17); cubo.setFace(11, 5, 0, 17);

        // Copiando os dados do cubo para parede e telhado *********************
        cubo.copiar(parede);
        cubo.copiar(telhado_esq);

        Transformacao::escala(telhado_esq, { 0.06, 0.46, 1 });
        Transformacao::cisalhamentoYX(telhado_esq, { PI / 4 });
        Transformacao::translacao(telhado_esq, { 0, 1, 0 });

        telhado_esq->copiar(telhado_dir);

        Transformacao::espelhoArbitrario(telhado_dir, Ponto{ 0.5, 0, 0 }, Vetor{ -1, 0, 0 });
        telhado_dir->inverterFaces();
    }
    ~Casa()
    {
        delete parede;
        delete telhado_esq;
        delete telhado_dir;
    }

    double intersecao(Raio raio) override
    {
        double t_parede = parede->intersecao(raio);
        double t_esq = telhado_esq->intersecao(raio);
        double t_dir = telhado_dir->intersecao(raio);
        
        double t = -1;

        if (t_parede > 0)
        {
            atingido = parede;
            material = parede->material;
            t = t_parede;
        }

        // atualizar o valor de t
        // apenas interessa se o t_esq for valido
        if (t_esq > 0) { 
            // se t for valido, verifica qual o menor valor
            if (t > 0) {
                // t recebe t_esq se for menor que o valor atual
                if (t_esq < t) {
                    atingido = telhado_esq;
                    material = telhado_esq->material;
                    t = t_esq;
                }
            // se t for invalido, apenas atualiza o valor
            } else {
                atingido = telhado_esq;
                material = telhado_esq->material;
                t = t_esq;
            } 
        }

        // atualizar o valor de t
        // apenas interessa se o t_dir for valido
        if (t_dir > 0) { 
            // se t for valido, verifica qual o menor valor
            if (t > 0) {
                // t recebe t_dir se for menor que o valor atual
                if (t_dir < t) {
                    atingido = telhado_dir;
                    material = telhado_dir->material;
                    t = t_dir;
                }
            // se t for invalido, apenas atualiza o valor
            } else {
                atingido = telhado_dir;
                material = telhado_dir->material;
                t = t_dir;
            } 
        }

        // retornando o valor de t que foi calculado
        return t;
    }

    Vetor getNormal(Ponto ponto) const override { return atingido->getNormal(ponto); }

    void transformar(Matriz matriz) override
    {
        parede->transformar(matriz);
        telhado_esq->transformar(matriz);
        telhado_dir->transformar(matriz);
    }
};

class Arvore : public Objeto
{
private:
    Cone *tronco;
    Esfera *folha;
    Objeto *atingido = nullptr;

public:
    Arvore(double altura, double raioFolha, double raioTronco, Material mf, Material mt)
    {
        Ponto centroFolha = {0, altura - raioFolha, 0};
        folha = new Esfera(centroFolha, raioFolha, mf);

        Ponto baseTronco = {0, 0, 0};
        Ponto vertTronco = {0, altura - raioFolha, 0};
        tronco = new Cone(baseTronco, vertTronco, raioTronco, mt);
    }
    ~Arvore()
    {
        delete folha;
        delete tronco;
    }

    double intersecao(Raio raio) override
    {
        double t_folha = folha->intersecao(raio);
        double t_tronco = tronco->intersecao(raio);

        double t = -1;

        if (t_folha > 0 and t_tronco > 0) {
            if (t_tronco < t_folha) {
                material = tronco->material;
                atingido = tronco;
                t = t_tronco;
            } else {
                material = folha->material;
                atingido = folha;
                t = t_folha;
            }
        } else if (t_folha > 0) {
            material = folha->material;
            atingido = folha;
            t = t_folha;
        } else if (t_tronco > 0) {
            material = tronco->material;
            atingido = tronco;
            t = t_tronco;
        } 
        else { t = -1; }
        
        // retornando o valor de t que foi calculado
        return t;
    }

    Vetor getNormal(Ponto ponto) const override { return atingido->getNormal(ponto); }

    void transformar(Matriz matriz) override
    {
        folha->transformar(matriz);
        tronco->transformar(matriz);
    }
};

class Poste : public Objeto
{
private:
    Cilindro *horizontal, *vertical;
    Objeto *atingido;

public:
    Poste(double alturaVer, double alturaHor, double raioVer, double raioHor, Material mp) {
    Ponto baseVer = Ponto{0, 0, 0};
    Vetor dirVer = Vetor{0, 1, 0};
    vertical = new Cilindro(baseVer, dirVer, raioVer, alturaVer, mp);

    Ponto baseHor = Ponto{0, alturaVer - raioHor, 0};
    Vetor dirHor = Vetor{1, 0, 0};
    horizontal = new Cilindro(baseHor, dirHor, raioHor, alturaHor, mp);

    material = mp;
    }
    ~Poste() {
        delete horizontal;
        delete vertical;
    };

    double intersecao (Raio raio)
    {
        double t_h = horizontal->intersecao(raio);
        double t_v = vertical->intersecao(raio);

        double t = -1;

        if (t_h > 0 and t_v > 0) {
            if (t_v < t_h) {
                atingido = vertical;
                t = t_v;
            } else {
                atingido = horizontal;
                t = t_h;
            }
        } else if (t_h > 0) {
            atingido = horizontal;
            t = t_h;
        } else if (t_v > 0) {
            atingido = vertical;
            t = t_v;
        } 
        else { t = -1; }

        // retornando o valor de t que foi calculado
        return t;
    }

    Vetor getNormal(Ponto ponto) const override { return atingido->getNormal(ponto); }

    void transformar(Matriz matriz) override
    {
        horizontal->transformar(matriz);
        vertical->transformar(matriz);
    }
};

class Textura : public Objeto
{
private:
    SDL_Surface *textura;
    Malha *plano;

    // Funcao que obtem a cor de um determinado pixel de uma textura
    Cor get_pixel_color(SDL_Surface *txt, int x, int y)
    {
        // obtendo o valor de bytes por pixel
        Uint8 bytesPixel = txt->format->BytesPerPixel;

        // Obtendo o endereco de um pixel especifico
        // txt->pixels = array contendo os pixels da textura 
        // txt->pitch  = tamanho de uma linha de pixels em bytes 
        // x, y	       = posicao do pixel a ser obtido da textura
        //               (0, 0) canto superior esquerdo da imagem
        Uint8 *pixel = (Uint8*) txt->pixels + y * txt->pitch + x * bytesPixel;
        Uint32 dados_pixel = *(Uint32*) pixel;

        SDL_Color color = {0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE};

        // Retrieve the RGB values of the specific pixel
        SDL_GetRGB(dados_pixel, txt->format, &color.r, &color.g, &color.b);

        return { color.r, color.g, color.b };
    }

public:
    Textura(Ponto p0, Ponto p1, Ponto p2, Ponto p3, SDL_Surface *txt)
    { // Assume ordem antihoraria com p0 sendo o vertice inferior esquerdo
        // Salvando ponteiro para a textura
        textura = txt;

        // Criando malha da textura
        plano = new Malha(4, 5, 2, {});

        // Definindo os vertices da malha na ordem que foi assumida
        plano->setVertice(0, p0);
        plano->setVertice(1, p1);
        plano->setVertice(2, p2);
        plano->setVertice(3, p3);

        // Definindo as arestas da malha na ordem assumida
        plano->setAresta(0, 0, 1);
        plano->setAresta(1, 1, 2);
        plano->setAresta(2, 2, 3);
        plano->setAresta(3, 3, 0);
        plano->setAresta(4, 0, 2);

        // Definindo as faces da malha na ordem antihoraria
        plano->setFace(0, 0, 1, 4);
        plano->setFace(1, 2, 3, 4);
    }
    ~Textura() { delete plano; }

    double intersecao(Raio raio) override
    {
        // variavel que guarda o menor t de todas intersecoes
        double menor_t = -1;
        // variavel que guarda o valor de t de cada intersecao
        double t_int = -1;
        // variavel que guarda as coordenadas baricentricas dos pontos
        double c[][3] = {{-1, -1, -1}, {-1, -1, -1}};

        // percorrendo todas as faces da malha
        for (int i = 0; i < plano->numfaces; ++i)
        {
            // obtendo id das arestas da face de id "i"
            int a1 = plano->faces[i].a1;
            int a2 = plano->faces[i].a2;

            // id dos vertices 1 e 2 da aresta 1, somando +1 para evitar id = 0
            int v1_a1 = plano->arestas[a1].v1 + 1;
            int v2_a1 = plano->arestas[a1].v2 + 1;

            // id dos vertices 1 e 2 da aresta 2, somando +1 para evitar id = 0
            int v1_a2 = plano->arestas[a2].v1 + 1;
            int v2_a2 = plano->arestas[a2].v2 + 1;

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
            }
            else
            {
                v1 = v2_a2;
                v2 = v1_a2;
                v3 = n1 / v1;
            }
            // reajustando o valor dos ids para o original
            --v1;
            --v2;
            --v3;
            // apos if e else, v1 tem o id do vertice comum
            // v2 e v3 tem o id dos vertices vizinhos na ordem anti-horaria

            // definindo p1, p2 e p3 como os pontos dos ids v1, v2 e v3
            Ponto p1 = plano->vertices[v1];
            Ponto p2 = plano->vertices[v2];
            Ponto p3 = plano->vertices[v3];

            // definindo os vetores que partem do vertice comum v1
            Vetor r1 = p2 - p1;
            Vetor r2 = p3 - p1;

            // definindo vetor normal nao normalizado
            Vetor N = vetorial(r1, r2);
            double modulo = norma(N);

            // vetor normal unitario
            Vetor normal = N / modulo;

            // calculando se tem intersecao plano da face
            double denominador = escalar(normal, raio.getDirecao());

            // se denominador == 0, raio é paralelo ao plano
            if (denominador != 0)
            {
                Vetor w = raio.getOrigem() - p1;
                t_int = -1 * (escalar(normal, w) / denominador);
            }
            else
            {
                t_int = -1;
            }

            // se houve intersecao com plano, entao checa se pertence a face
            if (t_int > 0)
            {
                // calculando ponto intersectado
                Ponto p = raio.pontoIntersecao(t_int);

                // definindo a area total do triangulo
                double areatotal = modulo;

                // coordenadas baricentricas
                double c1, c2, c3;

                Vetor s1 = p1 - p;
                Vetor s2 = p2 - p;
                Vetor s3 = p3 - p;

                c1 = escalar(vetorial(s3, s1), normal);

                // se negativo entao intersecao invalida
                if (c1 < 0)
                    t_int = -1;
                else
                { // senao calcula proxima coordenada baricentrica
                    c1 = c1 / areatotal;
                    c2 = escalar(vetorial(s1, s2), normal);

                    // se negativo entao intersecao invalida
                    if (c2 < 0)
                        t_int = -1;
                    else
                    { // senao calcula proxima coordenada baricentrica
                        c2 = c2 / areatotal;
                        c3 = 1 - (c1 + c2);

                        // se negativo entao intersecao invalida
                        if (c3 < 0)
                            t_int = -1;
                        else // senao o t_int já é valido
                        {    // salve as coordenadas baricentricas
                            c[i][0] = c1;
                            c[i][1] = c2;
                            c[i][2] = c3;
                        }
                    }
                }
            }
            // ATUALIZAR O VALOR DE menor_t
            // apenas interessa se o t_int for positivo
            if (t_int > 0)
            {
                // se menor_t for positivo, verifica qual o menor valor
                if (menor_t > 0)
                {
                    if (t_int < menor_t)
                    {
                        // menor_t recebe t_int se for menor que o valor atual
                        menor_t = t_int;
                        // atualiza a face atingida atual
                        plano->face_atingida = i;
                    }
                }
                // se menor_t for invalido, apenas atualiza o valor
                else
                {
                    menor_t = t_int;
                    plano->face_atingida = i;
                }
            }
        }

        int face = plano->face_atingida;
        // Salvando o material da coordenada atingida
        if (menor_t > 0)
        {
            double u, v;

            if (face == 0)
            {   // Face 0: (ordem antihoraria)
                // Vertice 1 -> c[face][1] - (u: 0, v: 0)
                // Vertice 2 -> c[face][2] - (u: 1, v: 0)
                // Vertice 3 -> c[face][0] - (u: 1, v: 1)
                u = (c[face][2] * 1) + (c[face][0] * 1); // v1: 0, v2: 1, v3: 1
                v = (c[face][0] * 1);                    // v1: 0, v2: 0, v3: 1
            }
            else 
            {   // Face 1: (ordem antihoraria)
                // Vertice 3 -> c[face][1] - (u: 1, v: 1)
                // Vertice 4 -> c[face][2] - (u: 0, v: 1)
                // Vertice 1 -> c[face][0] - (u: 0, v: 0)
                u = (c[face][1] * 1);                    // v3: 1, v4: 0, v1: 0
                v = (c[face][1] * 1) + (c[face][2] * 1); // v3: 1, v4: 1, v1: 0
            }

            // posicao do pixel nas colunas
            int x_pixel = floor((u * (textura->w - 1) + 0.5));
            // posicao do pixel nas linhas
            int y_pixel = floor((v * (textura->h - 1) + 0.5));

            // como na imagem o y cresce do topo a base, deve inverter o valor 
            // y do pixel da seguinte forma 
            y_pixel = textura->h - y_pixel - 1; 

            Cor cor_pixel = get_pixel_color(textura, x_pixel, y_pixel);

            Vetor ka = { cor_pixel.r / 255.0, cor_pixel.g / 255.0, cor_pixel.b / 255.0 };
            Vetor kd = ka;
            Vetor ke = ka; 

            material = { ka, kd, ke, 1 };
        }
        // retornando o valor do menor t calculado
        return menor_t;
    }

    Vetor getNormal(Ponto p) const override { return plano->getNormal(p); }

    void transformar(Matriz matriz) override { plano->transformar(matriz); }
};

#endif