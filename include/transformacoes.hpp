#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "objetos.hpp"

namespace Transformacao
{
    void translacao (Objeto *obj, Vetor t) 
    {
        Matriz T = identidade(4);
        T(0, 3) = t.x; 
        T(1, 3) = t.y; 
        T(2, 3) = t.z; 

        obj->transformar(T); 
    }
    
    void escala (Objeto *obj, Vetor s) 
    {
        Matriz T = identidade(4);
        T(0, 0) = s.x; 
        T(1, 1) = s.y; 
        T(2, 2) = s.z;

        obj->transformar(T);
    }

    void rotacaoX (Objeto *obj, double angulo) 
    {
        double seno = sin(angulo);
        double cosseno = cos(angulo);
        
        Matriz X = identidade(4);
        X(1, 1) = cosseno; X(1, 2) = -seno; 
        X(2, 1) = seno; X(2, 2) = cosseno; 

        obj->transformar(X);
    }

    void rotacaoY (Objeto *obj, double angulo) 
    {
        double seno = sin(angulo);
        double cosseno = cos(angulo);

        Matriz Y = identidade(4);
        Y(0, 0) = cosseno; Y(0, 2) = seno; 
        Y(2, 0) = -seno; Y(2, 2) = cosseno; 

        obj->transformar(Y);
    }

    void rotacaoZ (Objeto *obj, double angulo) 
    {
        double seno = sin(angulo);
        double cosseno = cos(angulo);

        Matriz Z  = identidade(4);
        Z(0, 0) = cosseno; Z(0, 1) = -seno;  
        Z(1, 0) = seno; Z(1, 1) = cosseno; 

        obj->transformar(Z);
    }

    void rotacaoArbitrario (Objeto *obj, Ponto p1, Ponto p2, double angulo) 
    {
        Vetor k = unitario(p2 - p1);
        Vetor i = vetorial({ 0, 1, 0 }, k);
        Vetor j = vetorial(k, i);

        // Definindo da matriz 4x4 do mundo para coordenadas do ponto
        Matriz w2c (4, 4);
        // Definindo a primeira linha da matriz
        w2c(0, 0) = i.x; w2c(0, 1) = i.y; w2c(0, 2) = i.z; 
        w2c(0, 3) = (-1) * escalar(i, { p1.x, p1.y, p1.z });
        // Definindo a segunda linha da matriz
        w2c(1, 0) = j.x; w2c(1, 1) = j.y; w2c(1, 2) = j.z; 
        w2c(1, 3) = (-1) * escalar(j, { p1.x, p1.y, p1.z });
        // Definindo a terceira linha da matriz
        w2c(2, 0) = k.x; w2c(2, 1) = k.y; w2c(2, 2) = k.z; 
        w2c(2, 3) = (-1) * escalar(k, { p1.x, p1.y, p1.z });
        // Definindo a quarta linha da matriz
        w2c(3, 0) = 0; w2c(3, 1) = 0; w2c(3, 2) = 0; w2c(3, 3) = 1;

        // Definindo da matriz 4x4 de coordenadas do ponto para mundo
        Matriz c2w (4, 4);
        // Definindo a primeira linha da matriz
        c2w(0, 0) = i.x; c2w(0, 1) = j.x; c2w(0, 2) = k.x; c2w(0, 3) = p1.x;
        // Definindo a segunda linha da matriz
        c2w(1, 0) = i.y; c2w(1, 1) = j.y; c2w(1, 2) = k.y; c2w(1, 3) = p1.y;
        // Definindo a terceira linha da matriz
        c2w(2, 0) = i.z; c2w(2, 1) = j.z; c2w(2, 2) = k.z; c2w(2, 3) = p1.z;
        // Definindo a quarta linha da matriz
        c2w(3, 0) = 0; c2w(3, 1) = 0; c2w(3, 2) = 0; c2w(3, 3) = 1;

        obj->transformar(w2c);
        rotacaoZ(obj, angulo);
        obj->transformar(c2w);
    }

    void espelhoYZ (Objeto *obj) 
    {
        Matriz YZ = identidade(4);
        YZ(0, 0) = -1;

        obj->transformar(YZ);
    }

    void espelhoXZ (Objeto *obj) 
    {
        Matriz XZ = identidade(4);
        XZ(1, 1) = -1;

        obj->transformar(XZ);
    }

    void espelhoXY (Objeto *obj) 
    {
        Matriz XY = identidade(4);
        XY(2, 2) = -1;

        obj->transformar(XY);
    }

    void espelhoArbitrario (Objeto *obj, Ponto p, Vetor n) 
    {
        translacao(obj, { -p.x, -p.y, -p.z });
        // Montando matriz que faz o espelho
        Matriz I = identidade(3);
        Matriz N (3, 1);
        N(0, 0) = n.x; N(1, 0) = n.y; N(2, 0) = n.z; 
        Matriz NT = transposta(N);

        Matriz R = I - (2 * N * NT);
        // Matriz de transformacao para espelho
        Matriz E = identidade(4);
        E(0, 0) = R(0, 0); E(0, 1) = R(0, 1); E(0, 2) = R(0, 2); 
        E(1, 0) = R(1, 0); E(1, 1) = R(1, 1); E(1, 2) = R(1, 2); 
        E(2, 0) = R(2, 0); E(2, 1) = R(2, 1); E(2, 2) = R(2, 2); 

        obj->transformar(E);
        translacao(obj, { p.x, p.y, p.z });
    }

    void cisalhamentoXY (Objeto *obj, double angulo) 
    {
        Matriz XY = identidade(4);
        XY(1, 0) = tan(angulo);
        obj->transformar(XY);
    }

    void cisalhamentoYX (Objeto *obj, double angulo) 
    {
        Matriz YX = identidade(4);
        YX(0, 1) = tan(angulo);
        obj->transformar(YX);
    }

    void cisalhamentoXZ (Objeto *obj, double angulo) 
    {
        Matriz XZ = identidade(4);
        XZ(2, 0) = tan(angulo);
        obj->transformar(XZ);
    }

    void cisalhamentoZX (Objeto *obj, double angulo) 
    {
        Matriz ZX = identidade(4);
        ZX(0, 2) = tan(angulo);
        obj->transformar(ZX);
    }

    void cisalhamentoYZ (Objeto *obj, double angulo) 
    {
        Matriz YZ = identidade(4);
        YZ(2, 1) = tan(angulo);
        obj->transformar(YZ);
    }

    void cisalhamentoZY (Objeto *obj, double angulo) 
    {
        Matriz ZY = identidade(4);
        ZY(1, 2) = tan(angulo);
        obj->transformar(ZY);
    }
} // namespace Transformacao

#endif