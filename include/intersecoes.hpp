#ifndef INTERSECOES_HPP
#define INTERSECOES_HPP

#include "objetos.hpp"
#include "basics.hpp"

double Esfera::intersecao (Raio raio) 
{
    Vetor w = raio.getOrigem() - Esfera::centro;

    double b = escalar( w, raio.getDirecao() );
    double c = escalar( w, w ) - (Esfera::raio * Esfera::raio);

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

double Plano::intersecao (Raio raio) 
{
    double denominador = escalar(Plano::normal, raio.getDirecao());

    if (denominador != 0)
    {
        Vetor w = raio.getOrigem() - Plano::ponto;
        double t_int = -1 * (escalar(Plano::normal, w) / denominador);
        return t_int;
    }
    else { return -1; }
}

double Cilindro::intersecao (Raio raio) 
{
    // guarda o valor do menor t que intersecta cilindro
    double menor_t = -1;
    // guarda o valor do t para cada intersecao
    double t_int = -1;
    // vetor auxiliar "w"
    Vetor w = raio.getDirecao() - (escalar(raio.getDirecao(), Cilindro::direcao) * Cilindro::direcao);
    // coeficiente "a" da equacao de segundo grau
    double a = escalar(w, w);

    // CHECAR INTERSECAO COM CILINDRO
    // a == 0 significa que o raio é paralelo a superficie cilindrica
    if (a != 0)
    {
        Vetor u = raio.getOrigem() - Cilindro::base;
        Vetor v = u - (escalar(u, Cilindro::direcao) * Cilindro::direcao);

        // coeficiente "b" da equacao de segundo grau 
        double b = escalar(v, w);
        // coeficiente "c" da equacao de segundo grau 
        double c = escalar(v, v) - (Cilindro::raio * Cilindro::raio);
        
        // delta da equacao de segundo grau 
        double delta = (b * b) - (a * c);

        // delta >= 0, significa que tem intersecao
        if (delta >= 0) { 
            // duas raizes da equacao (podem ser a mesma raiz)
            double t1 = (-b - sqrt(delta)) / a;
            double t2 = (-b + sqrt(delta)) / a;

            // obtendo tamanho da projecao do ponto sobre eixo do cilindro
            double escalar1 = escalar(raio.pontoIntersecao(t1) - Cilindro::base, Cilindro::direcao);
            double escalar2 = escalar(raio.pontoIntersecao(t2) - Cilindro::base, Cilindro::direcao);

            // se ponto de t1 for valido, atualiza o valor de t pois é a menor raiz
            if (escalar1 >= 0 && escalar1 <= Cilindro::altura) { t_int = t1; } 
            // se ponto de t2 for valido, atualiza o valor de t pois é a segunda raiz
            else if (escalar2 >= 0 && escalar2 <= Cilindro::altura) { t_int = t2; }
            // se nao houver ponto valido, deixa t com valor invalido
            else { t_int = -1; }
        }
        else { t_int = -1; } // se delta negativo, deixa t com valor invalido
    } else { t_int = -1; } // como a == 0, deixa t com valor invalido

    // atualiza menor_t com o valor de t encontrado
    if(t_int > 0) menor_t = t_int;

    // denominador no calculo da intersecao com planos
    double denominador;

    // CHECAR INTERSECAO COM BASE
    // escalar entre a normal da base e a direcao do raio
    denominador = escalar((-1 * Cilindro::direcao), raio.getDirecao());

    // se denominador != 0 tem intersecao com a base
    if (denominador != 0) {
        // verificando intersecao com base do cilindro
        double t_base = escalar((-1 * Cilindro::direcao), (raio.getOrigem() - Cilindro::base));
        t_base = -1 * (t_base / denominador);

        // se valor de t_base for valido
        if (t_base > 0) {
            // cria vetor do ponto intersectado ate o centro da base
            Vetor v = raio.pontoIntersecao(t_base) - Cilindro::base;
            // verificando se o ponto pertence ao circulo da base
            if (escalar(v, v) <= (Cilindro::raio * Cilindro::raio)) 
                t_int = t_base;
            else // se nao pertencer, deixa t_int com valor invalido
                t_int = -1;
        // se o valor de t_base nao for valido, deixa t_int com valor invalido
        } else { t_int = -1; } 
    // se denominador == 0 nao tem intersecao com a base
    } else { t_int = -1; }

    // ATUALIZAR O VALOR DE menor_t
    // apenas interessa se o t_int for valido
    if (t_int > 0) {
        // se menor_t for valido, verifica qual o menor valor
        if (menor_t > 0) 
            // menor_t recebe t_int se for menor que o valor atual
            menor_t = (t_int < menor_t ? t_int : menor_t);
        // se for invalido, apenas atualiza o valor
        else menor_t = t_int;
    }

    // CHECAR INTERSECAO COM TOPO
    // escalar entre a normal do topo e a direcao do raio
    denominador = escalar(Cilindro::direcao, raio.getDirecao());

    // se denominador != 0 tem intersecao com o topo
    if (denominador != 0) {
        // verificando intersecao com topo do cilindro
        double t_topo = escalar(Cilindro::direcao, (raio.getOrigem() - Cilindro::topo));
        t_topo = -1 * (t_topo / denominador);

        // se valor de t_topo for valido
        if (t_topo > 0) {
            // cria vetor do ponto intersectado ate o centro do topo
            Vetor v = raio.pontoIntersecao(t_topo) - Cilindro::topo;
            // verificando se o ponto pertence ao circulo do topo
            if (escalar(v, v) <= (Cilindro::raio * Cilindro::raio)) 
                t_int = t_topo;
            else // se nao pertencer, deixa t_int com valor invalido
                t_int = -1;
        // se o valor de t_topo for invalido, deixa t_int com valor invalido
        } else { t_int = -1; } 
    // se denominador == 0 nao tem intersecao com o topo
    } else { t_int = -1; }

    // ATUALIZAR O VALOR DE menor_t
    // apenas interessa se o t_int for valido
    if (t_int > 0) {
        // se menor_t for valido, verifica qual o menor valor
        if (menor_t > 0) 
            // menor_t recebe t_int se for menor que o valor atual
            menor_t = (t_int < menor_t ? t_int : menor_t);
        // se for invalido, apenas atualiza o valor
        else menor_t = t_int;
    }
    // retorna o menor valor de t encontrado
    return menor_t;
}

double Cone::intersecao (Raio raio) 
{
    // guarda o valor do menor t que intersecta cone
    double menor_t = -1;
    // guarda o valor do t para cada intersecao
    double t_int = -1;

    // variavel B das formulas 
    double beta = (Cone::altura * Cone::altura) / (Cone::raio * Cone::raio);
    // variavel w das formulas
    Vetor v = Cone::vertice - raio.getOrigem();
    // Produto escalar entre direcao do raio e direcao do cone
    double escalarRC = escalar(raio.getDirecao(), Cone::direcao);
    // Produto escalar entre vetor v e direcao do cone
    double escalarVC = escalar(v, Cone::direcao);
    // Produto escalar entre vetor v e direcao do raio
    double escalarVR = escalar(v, raio.getDirecao());

    // coeficiente "a" da equacao de segundo grau 
    double a = beta - (1 + beta) * (escalarRC * escalarRC);
    // coeficiente "b" da equacao de segundo grau 
    double b = (-beta) * escalarVR + (1 + beta) * escalarVC * escalarRC;
    // coeficiente "c" da equacao de segundo grau 
    double c = beta * escalar(v, v) - (1 + beta) * (escalarVC * escalarVC);

    // CHECAR INTERSECAO COM SUPERFICIE DO CONE
    // delta da equacao de segundo grau 
    double delta = (b * b) - (a * c);

    // delta >= 0, significa que tem intersecao
    if (delta >= 0) 
    {
        // duas raizes da equacao (podem ser a mesma raiz)
        double t1 = (-b - sqrt(delta)) / a;
        double t2 = (-b + sqrt(delta)) / a;

        // obtendo tamanho da projecao do ponto sobre eixo do cilindro
        double escalar1 = escalar(Cone::vertice - raio.pontoIntersecao(t1), Cone::direcao);
        double escalar2 = escalar(Cone::vertice - raio.pontoIntersecao(t2), Cone::direcao);

        // se ponto de t1 for valido, atualiza o valor de t pois é a menor raiz
        if (escalar1 >= 0 && escalar1 <= Cone::altura) { t_int = t1; } 
        // se ponto de t2 for valido, atualiza o valor de t pois é a segunda raiz
        else if (escalar2 >= 0 && escalar2 <= Cone::altura) { t_int = t2; }
        // se nao houver ponto valido, deixa t com valor invalido
        else { t_int = -1; }            
    } else { t_int = -1; }

    // atualiza menor_t com o valor de t encontrado
    if(t_int > 0) menor_t = t_int;

    // CHECAR INTERSECAO COM BASE
    // escalar entre a normal da base e a direcao do raio
    double denominador = escalar((-1 * Cone::direcao), raio.getDirecao());

    // se denominador != 0 tem intersecao com a base
    if (denominador != 0) {
        // verificando intersecao com base do cilindro
        double t_base = escalar((-1 * Cone::direcao), (raio.getOrigem() - Cone::base));
        t_base = -1 * (t_base / denominador);

        // se valor de t_base for valido
        if (t_base > 0) {
            // cria vetor do ponto intersectado ate o centro da base
            Vetor v = raio.pontoIntersecao(t_base) - Cone::base;
            // verificando se o ponto pertence ao circulo da base
            if (escalar(v, v) <= (Cone::raio * Cone::raio)) 
                t_int = t_base;
            else // se nao pertencer, deixa t_int com valor invalido
                t_int = -1;
        // se o valor de t_base nao for valido, deixa t_int com valor invalido
        } else { t_int = -1; } 
    // se denominador == 0 nao tem intersecao com a base
    } else { t_int = -1; }

    // ATUALIZAR O VALOR DE menor_t
    // apenas interessa se o t_int for positivo
    if (t_int > 0) {
        // se menor_t for valido, verifica qual o menor valor
        if (menor_t > 0) 
            // menor_t recebe t_int se for menor que o valor atual
            menor_t = (t_int < menor_t ? t_int : menor_t);
        // se for invalido, apenas atualiza o valor
        else menor_t = t_int;
    }
    // retorna o menor valor de t encontrado
    return menor_t;
}

double Malha::intersecao (Raio raio) 
{
    // variavel que guarda o menor t de todas intersecoes
    double menor_t = -1;
    // variavel que guarda o valor de t de cada intersecao
    double t_int = -1; 

    // percorrendo todas as faces da malha
    for (int i = 0; i < Malha::numfaces; ++i)
    {
        // obtendo id das arestas da face de id "i"
        int a1 = Malha::faces[i].a1;
        int a2 = Malha::faces[i].a2;

        // id dos vertices 1 e 2 da aresta 1, somando +1 para evitar id = 0
        int v1_a1 = Malha::arestas[a1].v1 + 1; 
        int v2_a1 = Malha::arestas[a1].v2 + 1; 

        // id dos vertices 1 e 2 da aresta 2, somando +1 para evitar id = 0
        int v1_a2 = Malha::arestas[a2].v1 + 1; 
        int v2_a2 = Malha::arestas[a2].v2 + 1; 

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
        Ponto p1 = Malha::vertices[v1]; 
        Ponto p2 = Malha::vertices[v2]; 
        Ponto p3 = Malha::vertices[v3]; 

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
        else { t_int = -1; }

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
            if (c1 < 0) t_int = -1;
            else { // senao calcula proxima coordenada baricentrica
                c1 = c1 / areatotal;
                c2 = escalar(vetorial(s1, s2), normal);

                // se negativo entao intersecao invalida
                if (c2 < 0) t_int = -1;
                else { // senao calcula proxima coordenada baricentrica
                    c2 = c2 / areatotal;
                    c3 = 1 - (c1 + c2);

                    // se negativo entao intersecao invalida
                    if (c3 < 0) t_int = -1;
                    // senao nao precisa fazer nada pois o t_int já é valido
                }
            }
        }
        // ATUALIZAR O VALOR DE menor_t
        // apenas interessa se o t_int for positivo
        if (t_int > 0) {
            // se menor_t for positivo, verifica qual o menor valor
            if (menor_t > 0) {
                if (t_int < menor_t) 
                {
                    // menor_t recebe t_int se for menor que o valor atual
                    menor_t = t_int;
                    // atualiza a face atingida atual
                    Malha::face_atingida = i;
                }
            }
            // se menor_t for invalido, apenas atualiza o valor
            else { menor_t = t_int; Malha::face_atingida = i; }
        }
    }
    // retornando o valor do menor t calculado
    return menor_t;
}

#endif