#pragma once

#include <vector>

// x[b][o] = masa del bloque b procesada bajo el modo o.
// Indices consistentes con el orden de Instance::blocks e Instance::modes.
using Solution = std::vector<std::vector<double>>;
