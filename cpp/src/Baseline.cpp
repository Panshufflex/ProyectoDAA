#include "Baseline.hpp"

#include <algorithm>
#include <cstddef>
#include <vector>

Solution runBaselineGreedy(const Instance& inst) {
    const int n = inst.numBlocks();
    const int k = inst.numModes();
    const int p = inst.numTypes();
    constexpr double EPS = 1e-9;

    Solution x(n, std::vector<double>(k, 0.0));
    std::vector<double> remMass(n);
    for (int b = 0; b < n; ++b) remMass[b] = inst.blocks[b].mass;
    double remTime = inst.q;

    // v_bo/m_b es el valor por unidad de masa de procesar el bloque b bajo
    // el modo o: es la misma razon que ordena el knapsack fraccionario
    // clasico. Esta linea base ignora la restriccion de mezcla mientras
    // ordena y asigna, y solo la corrige despues (ver mas abajo), a
    // diferencia de la propuesta principal, que nunca la rompe.
    struct Pair {
        int b;
        int o;
        double ratio;
    };
    std::vector<Pair> pairs;
    pairs.reserve(static_cast<std::size_t>(n) * static_cast<std::size_t>(k));
    for (int b = 0; b < n; ++b) {
        for (int o = 0; o < k; ++o) {
            pairs.push_back({b, o, inst.blocks[b].value[o] / inst.blocks[b].mass});
        }
    }
    std::sort(pairs.begin(), pairs.end(), [](const Pair& a, const Pair& c) {
        return a.ratio > c.ratio;
    });

    for (const auto& pr : pairs) {
        const int b = pr.b;
        const int o = pr.o;
        if (remMass[b] <= EPS || remTime <= EPS) continue;

        const double rate = inst.modes[o].rate;
        const double cantidad = std::min(remMass[b], remTime * rate);
        x[b][o] += cantidad;
        remMass[b] -= cantidad;
        remTime -= cantidad / rate;

        // Correccion de mezcla: escala hacia abajo cada tipo de roca para
        // respetar w_op, usando como referencia el tipo mas limitante.
        // La masa liberada por este escalado no se recupera (limitacion
        // de esta linea base frente a la propuesta principal).
        std::vector<double> massByType(p, 0.0);
        for (int bb = 0; bb < n; ++bb) massByType[inst.blocks[bb].typeIndex] += x[bb][o];

        // bottleneck = cuanto se puede escalar "hacia arriba como maximo"
        // cada tipo de roca (masa/w) sin pasarse del tipo mas escaso.
        // Se usa el minimo entre tipos porque ese es el que limita: los
        // demas tipos deben bajar hasta esa misma proporcion.
        double bottleneck = 0.0;
        bool any = false;
        for (int t = 0; t < p; ++t) {
            if (inst.modes[o].w[t] <= EPS) continue;
            const double c = massByType[t] / inst.modes[o].w[t];
            if (!any || c < bottleneck) {
                bottleneck = c;
                any = true;
            }
        }
        if (!any) continue; // modo sin requerimiento de mezcla definido

        for (int t = 0; t < p; ++t) {
            if (massByType[t] <= EPS) continue;
            const double target = bottleneck * inst.modes[o].w[t];
            const double factor = target / massByType[t];
            if (factor < 1.0 - 1e-12) {
                for (int bb = 0; bb < n; ++bb) {
                    if (inst.blocks[bb].typeIndex == t) x[bb][o] *= factor;
                }
            }
        }
    }

    return x;
}
