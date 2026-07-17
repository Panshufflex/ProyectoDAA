#pragma once

#include <string>
#include <vector>

// Modo operacional: nombre, tasa de procesamiento r_o (ton/h) y fraccion
// requerida w[o][p] de cada tipo de roca (mismo orden que Instance::rockTypes).
struct Mode {
    std::string name;
    double rate = 0.0;
    std::vector<double> w;
};

// Bloque de mineral: tipo de roca (indice en rockTypes), masa maxima m_b
// y valor economico value[o] = v_bo bajo cada modo o.
struct Block {
    int id = 0;
    int typeIndex = 0;
    double mass = 0.0;
    std::vector<double> value;
};

// Instancia del problema de procesamiento mineral con modos operacionales
// (conjuntos B, O, P y parametros m_b, v_bo, r_o, q, w_op de la formulacion
// de referencia).
class Instance {
public:
    std::vector<std::string> rockTypes;
    std::vector<Mode> modes;
    std::vector<Block> blocks;
    double q = 0.0;

    int numBlocks() const { return static_cast<int>(blocks.size()); }
    int numModes() const { return static_cast<int>(modes.size()); }
    int numTypes() const { return static_cast<int>(rockTypes.size()); }

    // Carga una instancia desde un archivo de texto plano con el formato:
    //   ROCKTYPES <p>
    //   <tipo_1> ... <tipo_p>
    //   MODES <k>
    //   <nombre_modo> <r_o> <w_o,1> ... <w_o,p>   (una linea por modo)
    //   TIME <q>
    //   BLOCKS <n>
    //   <id> <tipo> <masa> <v_modo1> ... <v_modok>   (una linea por bloque)
    static Instance loadFromFile(const std::string& path);
};
