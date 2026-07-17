#pragma once

#include <string>
#include <vector>

#include "Instance.hpp"
#include "Types.hpp"

// Resultado de verificar una solucion contra las restricciones (6)-(9)
// del modelo de referencia.
struct FeasibilityReport {
    bool feasible = true;
    double timeUsed = 0.0;
    std::vector<std::string> violations;
};

// Verifica que x cumpla: (6) tiempo total de planta, (7) masa maxima por
// bloque, (8) proporciones de mezcla por tipo de roca en cada modo, y
// (9) no negatividad. eps es la tolerancia numerica de factibilidad.
FeasibilityReport verify(const Instance& inst, const Solution& x, double eps = 1e-6);
