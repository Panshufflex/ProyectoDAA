#include "Verifier.hpp"

#include <algorithm>
#include <cmath>

FeasibilityReport verify(const Instance& inst, const Solution& x, double eps) {
    FeasibilityReport report;
    const int n = inst.numBlocks();
    const int k = inst.numModes();
    const int p = inst.numTypes();

    // (9) No negatividad.
    for (int b = 0; b < n; ++b) {
        for (int o = 0; o < k; ++o) {
            if (x[b][o] < -eps) {
                report.feasible = false;
                report.violations.push_back(
                    "x_bo negativo en el bloque " + std::to_string(inst.blocks[b].id));
            }
        }
    }

    // (7) Masa maxima por bloque.
    for (int b = 0; b < n; ++b) {
        double total = 0.0;
        for (int o = 0; o < k; ++o) total += x[b][o];
        if (total > inst.blocks[b].mass + eps) {
            report.feasible = false;
            report.violations.push_back(
                "El bloque " + std::to_string(inst.blocks[b].id) + " excede su masa maxima");
        }
    }

    // (6) Tiempo total de planta.
    double timeUsed = 0.0;
    for (int b = 0; b < n; ++b) {
        for (int o = 0; o < k; ++o) {
            timeUsed += x[b][o] / inst.modes[o].rate;
        }
    }
    report.timeUsed = timeUsed;
    if (timeUsed > inst.q + eps) {
        report.feasible = false;
        report.violations.push_back("Se excede el tiempo total de planta disponible (q)");
    }

    // (8) Proporciones de mezcla por tipo de roca, para cada modo.
    //
    // La restriccion original compara TODOS los pares (p1, p2), pero eso
    // es redundante: si masa_p / w_p es igual a una misma constante para
    // cada tipo p, entonces es igual entre cualquier par de tipos. Por
    // eso alcanza con fijar un tipo de referencia (refType, el primero
    // con w_op > 0) y comparar cada tipo contra el, en vez de comparar
    // cada par entre si (evita un chequeo O(P^2) innecesario).
    for (int o = 0; o < k; ++o) {
        std::vector<double> massByType(p, 0.0);
        for (int b = 0; b < n; ++b) massByType[inst.blocks[b].typeIndex] += x[b][o];

        int refType = -1;
        for (int t = 0; t < p; ++t) {
            if (inst.modes[o].w[t] > eps) { refType = t; break; }
        }
        if (refType == -1) continue; // modo sin requerimiento de mezcla definido

        double refRatio = massByType[refType] / inst.modes[o].w[refType];
        for (int t = 0; t < p; ++t) {
            if (inst.modes[o].w[t] <= eps) {
                if (massByType[t] > eps) {
                    report.feasible = false;
                    report.violations.push_back(
                        "El modo " + inst.modes[o].name +
                        " procesa un tipo de roca con w_op = 0");
                }
                continue;
            }
            double ratio = massByType[t] / inst.modes[o].w[t];
            double scale = std::max({std::abs(refRatio), std::abs(ratio), 1.0});
            if (std::abs(ratio - refRatio) > eps * scale) {
                report.feasible = false;
                report.violations.push_back(
                    "El modo " + inst.modes[o].name + " no respeta la proporcion de mezcla exigida");
            }
        }
    }

    return report;
}
