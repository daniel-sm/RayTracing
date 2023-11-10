#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "objetos.hpp"

namespace Transformacao
{
    void translacao (Objeto *obj, Vetor t) 
    {
        Matriz T (4, 4);

        T(0, 0) = 1; T(0, 1) = 0; T(0, 2) = 0; T(0, 3) = t.x; 
        T(1, 0) = 0; T(1, 1) = 1; T(1, 2) = 0; T(1, 3) = t.y; 
        T(2, 0) = 0; T(2, 1) = 0; T(2, 2) = 1; T(2, 3) = t.z; 
        T(3, 0) = 0; T(3, 1) = 0; T(3, 2) = 0; T(3, 3) = 1; 

        obj->transformar(T); 
    }
    
    void escala (Objeto *obj, Vetor s) 
    {
        Matriz T (4, 4);

        T(0, 0) = s.x; T(0, 1) = 0;   T(0, 2) = 0;   T(0, 3) = 0; 
        T(1, 0) = 0;   T(1, 1) = s.y; T(1, 2) = 0;   T(1, 3) = 0; 
        T(2, 0) = 0;   T(2, 1) = 0;   T(2, 2) = s.z; T(2, 3) = 0; 
        T(3, 0) = 0;   T(3, 1) = 0;   T(3, 2) = 0;   T(3, 3) = 1; 

        obj->transformar(T);
    }

} // namespace Transformacao

#endif