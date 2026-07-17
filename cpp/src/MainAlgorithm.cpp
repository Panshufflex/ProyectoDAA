#include "MainAlgorithm.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

namespace {

constexpr double EPS = 1e-9;

// Para cada (modo, tipo de roca): lista de indices de bloques de ese tipo,
// ordenados en forma descendente por v_bo/m_b bajo ese modo, y un puntero
// al mejor bloque todavia disponible (masa restante > 0).
struct TypePointer {
    std::vector<int> sortedBlocks;
    std::size_t ptr = 0;
};

} // namespace

Solution runMainAlgorithm(const Instance& inst) {
    const int n = inst.numBlocks();
    const int k = inst.numModes();
    const int p = inst.numTypes();

    Solution x(n, std::vector<double>(k, 0.0));
    std::vector<double> remMass(n);
    for (int b = 0; b < n; ++b) remMass[b] = inst.blocks[b].mass;
    double remTime = inst.q;

    // tp[o][t] = puntero ordenado a los bloques de tipo t bajo el modo o.
    std::vector<std::vector<TypePointer>> tp(
        k, std::vector<TypePointer>(p));
    for (int o = 0; o < k; ++o) {
        for (int t = 0; t < p; ++t) {
            std::vector<int> idx;
            for (int b = 0; b < n; ++b) {
                if (inst.blocks[b].typeIndex == t) idx.push_back(b);
            }
            std::sort(idx.begin(), idx.end(), [&](int a, int c) {
                return inst.blocks[a].value[o] / inst.blocks[a].mass >
                       inst.blocks[c].value[o] / inst.blocks[c].mass;
            });
            tp[o][t].sortedBlocks = std::move(idx);
        }
    }

    // Devuelve el indice del mejor bloque de tipo t, bajo el modo o, que
    // todavia tenga masa disponible (o -1 si no queda ninguno).
    auto mejorBloque = [&](int o, int t) -> int {
        auto& list = tp[o][t].sortedBlocks;
        auto& ptr = tp[o][t].ptr;
        while (ptr < list.size() && remMass[list[ptr]] <= EPS) ++ptr;
        if (ptr >= list.size()) return -1;
        return list[ptr];
    };

    // Valor por hora que generaria el modo o si se le asignara el
    // siguiente tramo de tiempo ahora mismo, usando los mejores bloques
    // disponibles de cada tipo de roca. -infinito si falta algun tipo
    // esencial (w_op > 0 sin bloques disponibles).
    auto tasaModo = [&](int o) -> double {
        double valorPorMasa = 0.0;
        for (int t = 0; t < p; ++t) {
            if (inst.modes[o].w[t] <= EPS) continue;
            const int b = mejorBloque(o, t);
            if (b < 0) return -std::numeric_limits<double>::infinity();
            valorPorMasa += inst.modes[o].w[t] * (inst.blocks[b].value[o] / inst.blocks[b].mass);
        }
        return valorPorMasa * inst.modes[o].rate;
    };

    // Cola de prioridad de modos por tasa actual (valor/hora). Como los
    // modos comparten bloques (el mismo bloque puede ser el "mejor
    // disponible" de varios modos a la vez), procesar un modo puede dejar
    // obsoleta la tasa cacheada de otro modo que sigue en la cola. En vez
    // de actualizar todas las entradas afectadas al vuelo (costoso), se
    // usa "borrado perezoso": cada vez que se saca el maximo de la cola,
    // se recalcula su tasa real: si no coincide con la que tenia cuando
    // se encolo, se vuelve a insertar con el valor correcto y se sigue
    // buscando el verdadero maximo actual.
    using Entry = std::pair<double, int>; // (tasa, modo)
    std::priority_queue<Entry> heap;
    for (int o = 0; o < k; ++o) {
        const double r = tasaModo(o);
        if (r > EPS) heap.push({r, o});
    }

    while (remTime > EPS && !heap.empty()) {
        const Entry top = heap.top();
        heap.pop();
        const double tasa = top.first;
        const int o = top.second;

        const double actual = tasaModo(o);
        if (std::abs(actual - tasa) > 1e-6 * std::max(1.0, std::abs(tasa))) {
            // Entrada obsoleta: el estado cambio desde que se encolo.
            if (actual > EPS) heap.push({actual, o});
            continue;
        }
        if (actual <= EPS) continue;

        // deltaH = cuantas horas se le pueden dar a este modo antes de
        // que algo cambie: se agota el tiempo total, o se agota el mejor
        // bloque disponible de alguno de los tipos que este modo necesita
        // (el minimo de todos los limites posibles, igual que en el
        // fractional knapsack clasico pero con varios recursos a la vez).
        double deltaH = remTime;
        for (int t = 0; t < p; ++t) {
            if (inst.modes[o].w[t] <= EPS) continue;
            const int b = mejorBloque(o, t);
            const double limite = remMass[b] / (inst.modes[o].w[t] * inst.modes[o].rate);
            deltaH = std::min(deltaH, limite);
        }
        if (deltaH <= EPS) continue;

        // Se reparte deltaH horas del modo o entre los tipos de roca
        // exactamente en la proporcion w_op: por eso la mezcla nunca se
        // rompe, ni siquiera de forma transitoria (ver argumento de
        // validez en el informe de la Entrega 2).
        for (int t = 0; t < p; ++t) {
            if (inst.modes[o].w[t] <= EPS) continue;
            const int b = mejorBloque(o, t);
            const double cantidad = deltaH * inst.modes[o].w[t] * inst.modes[o].rate;
            x[b][o] += cantidad;
            remMass[b] -= cantidad;
        }
        remTime -= deltaH;

        const double nueva = tasaModo(o);
        if (nueva > EPS) heap.push({nueva, o});
    }

    return x;
}
