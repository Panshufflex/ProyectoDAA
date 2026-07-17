#include "Instance.hpp"

#include <fstream>
#include <stdexcept>
#include <unordered_map>

// El parser lee el archivo token a token con el operador >>, que ignora
// espacios y saltos de linea automaticamente: por eso el formato no exige
// que cada dato este en su propia linea, aunque los archivos de datos se
// escriben asi por legibilidad. Las secciones deben venir en este orden
// fijo (ROCKTYPES, MODES, TIME, BLOCKS) porque los indices de tipo de
// roca (typeIndex) y el numero de modos deben conocerse antes de poder
// leer las lineas de modos y de bloques que dependen de ellos.
Instance Instance::loadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("No se pudo abrir el archivo de instancia: " + path);
    }

    Instance inst;
    std::string tag;

    int numTypes = 0;
    in >> tag >> numTypes;
    if (tag != "ROCKTYPES") throw std::runtime_error("Formato invalido: se esperaba ROCKTYPES");
    // typeIndex traduce el nombre de tipo de roca (ej. "I") al indice p
    // usado en Block::typeIndex y en Mode::w[p], para no tener que buscar
    // por nombre en el resto del programa.
    std::unordered_map<std::string, int> typeIndex;
    for (int p = 0; p < numTypes; ++p) {
        std::string name;
        in >> name;
        typeIndex[name] = p;
        inst.rockTypes.push_back(name);
    }

    int numModes = 0;
    in >> tag >> numModes;
    if (tag != "MODES") throw std::runtime_error("Formato invalido: se esperaba MODES");
    for (int o = 0; o < numModes; ++o) {
        Mode mode;
        in >> mode.name >> mode.rate;
        mode.w.resize(numTypes);
        for (int p = 0; p < numTypes; ++p) in >> mode.w[p];
        inst.modes.push_back(mode);
    }

    in >> tag >> inst.q;
    if (tag != "TIME") throw std::runtime_error("Formato invalido: se esperaba TIME");

    int numBlocks = 0;
    in >> tag >> numBlocks;
    if (tag != "BLOCKS") throw std::runtime_error("Formato invalido: se esperaba BLOCKS");
    for (int i = 0; i < numBlocks; ++i) {
        Block block;
        std::string typeName;
        in >> block.id >> typeName >> block.mass;
        auto it = typeIndex.find(typeName);
        if (it == typeIndex.end()) {
            throw std::runtime_error("Tipo de roca desconocido en el bloque " + typeName);
        }
        block.typeIndex = it->second;
        block.value.resize(numModes);
        for (int o = 0; o < numModes; ++o) in >> block.value[o];
        inst.blocks.push_back(block);
    }

    if (!in) {
        throw std::runtime_error("Archivo de instancia incompleto o mal formado: " + path);
    }

    return inst;
}
