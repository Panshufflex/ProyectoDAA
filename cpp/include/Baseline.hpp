#pragma once

#include "Instance.hpp"
#include "Types.hpp"

// Linea base de la Entrega 1: heuristica greedy simple.
// Ordena los pares (bloque, modo) en forma descendente por razon
// valor/masa (v_bo/m_b), asigna secuencialmente respetando la masa y el
// tiempo disponibles, y corrige la mezcla bloque a bloque escalando hacia
// abajo el tipo de roca que sobrepasa la proporcion exigida. La masa que
// se libera al escalar NO se recupera: es una limitacion conocida de esta
// linea base frente a la propuesta principal (ver informe, seccion de
// dificultades).
Solution runBaselineGreedy(const Instance& inst);
