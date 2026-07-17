#pragma once

#include "Instance.hpp"
#include "Types.hpp"

// Propuesta algoritmica principal de la Entrega 2: construccion greedy con
// reparto proporcional entre modos operacionales ("subasta" por turnos).
// En cada paso, el modo con mayor tasa de retorno actual (valor por hora)
// recibe el siguiente tramo de tiempo de planta, consumiendo los mejores
// bloques disponibles de cada tipo de roca en la proporcion exacta que
// exige su mezcla, hasta que se agota el mejor bloque de algun tipo o se
// agota el tiempo total de planta. La solucion resultante es siempre
// factible por construccion (ver argumento de validez en el informe).
Solution runMainAlgorithm(const Instance& inst);
