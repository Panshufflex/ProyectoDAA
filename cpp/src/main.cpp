#include <chrono>
#include <cmath>
#include <exception>
#include <iomanip>
#include <iostream>
#include <string>

#include "Baseline.hpp"
#include "Instance.hpp"
#include "MainAlgorithm.hpp"
#include "Verifier.hpp"

namespace {

double totalValue(const Instance& inst, const Solution& x) {
    double total = 0.0;
    for (int b = 0; b < inst.numBlocks(); ++b) {
        for (int o = 0; o < inst.numModes(); ++o) {
            total += (inst.blocks[b].value[o] / inst.blocks[b].mass) * x[b][o];
        }
    }
    return total;
}

void printReport(const std::string& nombre, const Instance& inst, const Solution& x, double ms) {
    std::cout << "\n=== " << nombre << " ===\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Valor total: " << totalValue(inst, x) << "\n";
    std::cout << "Tiempo de ejecucion: " << ms << " ms\n";

    std::cout << "Masa procesada por modo:\n";
    for (int o = 0; o < inst.numModes(); ++o) {
        double m = 0.0;
        for (int b = 0; b < inst.numBlocks(); ++b) m += x[b][o];
        std::cout << "  " << inst.modes[o].name << ": " << m << " ton\n";
    }

    std::cout << "Masa procesada por tipo de roca:\n";
    for (int t = 0; t < inst.numTypes(); ++t) {
        double m = 0.0;
        for (int b = 0; b < inst.numBlocks(); ++b) {
            if (inst.blocks[b].typeIndex == t) {
                for (int o = 0; o < inst.numModes(); ++o) m += x[b][o];
            }
        }
        std::cout << "  " << inst.rockTypes[t] << ": " << m << " ton\n";
    }

    const FeasibilityReport report = verify(inst, x);
    std::cout << "Factibilidad: " << (report.feasible ? "VALIDA" : "NO VALIDA") << "\n";
    std::cout << "Tiempo de planta usado: " << report.timeUsed << " / " << inst.q << " h\n";
    for (const auto& v : report.violations) std::cout << "  - " << v << "\n";
}

} // namespace

int main(int argc, char** argv) {
    const std::string path = (argc > 1) ? argv[1] : "data/instancia_20bloques.txt";

    Instance inst;
    try {
        inst = Instance::loadFromFile(path);
    } catch (const std::exception& e) {
        std::cerr << "Error al cargar la instancia: " << e.what() << "\n";
        return 1;
    }

    std::cout << "Instancia cargada: " << inst.numBlocks() << " bloques, "
              << inst.numModes() << " modos, " << inst.numTypes() << " tipos de roca.\n";

    const auto t0 = std::chrono::high_resolution_clock::now();
    const Solution baseline = runBaselineGreedy(inst);
    const auto t1 = std::chrono::high_resolution_clock::now();
    const Solution principal = runMainAlgorithm(inst);
    const auto t2 = std::chrono::high_resolution_clock::now();

    const double msBaseline = std::chrono::duration<double, std::milli>(t1 - t0).count();
    const double msPrincipal = std::chrono::duration<double, std::milli>(t2 - t1).count();

    printReport("Linea base (greedy simple, Entrega 1)", inst, baseline, msBaseline);
    printReport("Propuesta principal (Entrega 2)", inst, principal, msPrincipal);

    const double vBase = totalValue(inst, baseline);
    const double vMain = totalValue(inst, principal);
    std::cout << "\n=== Comparacion ===\n";
    std::cout << std::fixed << std::setprecision(3);
    if (std::abs(vBase) > 1e-9) {
        std::cout << "Mejora de la propuesta principal sobre la linea base: "
                  << (vMain - vBase) / std::abs(vBase) * 100.0 << " %\n";
    } else {
        std::cout << "Diferencia absoluta de valor: " << (vMain - vBase) << "\n";
    }

    return 0;
}
