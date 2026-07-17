# Procesamiento mineral con modos operacionales — Entrega 2

Proyecto semestral de **Diseño y Análisis de Algoritmos** (Tema 2), Universidad Católica del Norte, primer semestre 2026.

## Descripción del trabajo

El problema consiste en decidir cuánta masa de cada bloque de mineral procesar bajo cada modo operacional de una planta, maximizando el valor económico recuperable, sujeto a:

1. el tiempo total de planta disponible ($q$),
2. la masa máxima de cada bloque ($m_b$),
3. las proporciones de mezcla exigidas entre tipos de roca en cada modo ($w_{op}$),
4. la no negatividad de las variables de decisión ($x_{bo}$, masa del bloque $b$ procesada bajo el modo $o$).

Es una variante de la mochila fraccionaria enriquecida con modos operacionales y restricciones de mezcla (*blending*). El enunciado completo y el marco general del proyecto están en los PDF de la carpeta raíz del repositorio.

Para esta entrega, el grupo definió:
- una **línea base** (Entrega 1): heurística greedy simple que ordena por razón valor/masa y corrige la mezcla bloque a bloque, sin recuperar la capacidad que pierde al corregir;
- una **propuesta algorítmica principal** (Entrega 2): una construcción greedy que reparte el tiempo de planta entre modos como una "subasta por turnos", manteniendo siempre la mezcla exacta y sin necesitar corrección posterior.

El razonamiento completo, con una explicación en lenguaje simple, un ejemplo numérico paso a paso, el pseudocódigo, el análisis de complejidad y el argumento de validez de la propuesta principal, está en **`PropuestaAlgoritmica_Entrega2.docx`** (carpeta raíz). Este README cubre solo la parte de implementación.

## Estructura del proyecto

```
README.md    (este archivo)
DatosTema2.xlsx, minerals-15-00427-v2.pdf, PropuestaAlgoritmica_Entrega2.docx
cpp/
  include/   Instance.hpp, Types.hpp, Verifier.hpp, Baseline.hpp, MainAlgorithm.hpp
  src/       Instance.cpp, Verifier.cpp, Baseline.cpp, MainAlgorithm.cpp, main.cpp
  data/      instancia_20bloques.txt
  Makefile
```

- **Instance**: representación computacional de la instancia (bloques, modos, tipos de roca) y su lector de archivo.
- **Baseline**: línea base de la Entrega 1.
- **MainAlgorithm**: propuesta principal de la Entrega 2.
- **Verifier**: verifica que una solución cumpla las restricciones (6)-(9) del modelo.
- **main.cpp**: corre ambos algoritmos sobre la misma instancia, mide tiempo de ejecución, verifica factibilidad e imprime la comparación.

El código tiene comentarios en los puntos donde el razonamiento no es obvio a simple vista (por qué se ordena por esa razón, por qué la cola de prioridad usa borrado perezoso, por qué el escalado nunca rompe la mezcla, etc.). Para el detalle completo del diseño, ver el docx mencionado arriba.

## Software utilizado

- **C++17**, sin librerías externas — solo la biblioteca estándar (`<vector>`, `<algorithm>`, `<queue>`, `<chrono>`, etc.).
- Compilador recomendado: **MinGW-w64 (GCC)**, instalado vía MSYS2 (`winget install -e --id MSYS2.MSYS2`, luego `pacman -S mingw-w64-ucrt-x86_64-gcc`). También funciona con clang o MSVC mientras soporten C++17.
- **Node.js**: se usó una sola vez, fuera de este proyecto de C++, para extraer los 20 bloques representativos desde `DatosTema2.xlsx`. No es una dependencia del programa — el resultado de ese proceso es `data/instancia_20bloques.txt`, ya generado.
- Datos y referencias fuente (carpeta raíz del repositorio): `DatosTema2.xlsx` (bloques reales del profesor) y `minerals-15-00427-v2.pdf` (Leiva, Lespay, Quelopana y Navarra, 2025, *Minerals* 15(4), 427 — paper de referencia del tema).

## Instrucciones para compilar y ejecutar

Con `make`:
```
cd cpp
make
./procesamiento_mineral data/instancia_20bloques.txt
```

Sin `make`, invocando el compilador directamente:
```
cd cpp
g++ -std=c++17 -O2 -Wall -Wextra -Iinclude src/*.cpp -o procesamiento_mineral
./procesamiento_mineral data/instancia_20bloques.txt
```

Si no se indica un archivo de instancia, el programa usa `data/instancia_20bloques.txt` por defecto. La salida incluye, para la línea base y la propuesta principal: valor total, masa procesada por modo y por tipo de roca, tiempo de ejecución, y verificación explícita de factibilidad.

## Qué falta por hacer

- [x] **Compilar y verificar** que el código corre sin errores. Verificado de forma remota (mismo compilador, GCC 16.1.0) con resultado correcto — ver "Resultados preliminares verificados" abajo. Igual hay que compilarlo localmente al menos una vez antes de dar por cerrada la entrega, para confirmar que también corre bien en la máquina de cada integrante.
- [ ] **Correr experimentos** con más instancias variando número de bloques, tipos de roca y modos, según el protocolo de comparación definido en la Entrega 1 (media y desviación estándar de valor y tiempo por categoría).
- [ ] Decidir si se implementa una **referencia exacta** (LP/Simplex) para instancias chicas, para medir la cercanía al óptimo (mencionado como opcional en la Entrega 1).
- [ ] Redactar el **informe completo de la Entrega 2**, integrando lo que ya está en `PropuestaAlgoritmica_Entrega2.docx` (propuesta, pseudocódigo, complejidad, validez) con los resultados experimentales, la identificación de dificultades y el plan de refinamiento para la Entrega 3 (ambos exigidos por el enunciado).
- [ ] Confirmar o documentar como supuesto el mapeo entre los tipos de roca "I"/"II" del Excel y los nombres reales del paper ("Diorite porphyry" / "Silicified intrusive breccia") — actualmente asumido como I = Diorite, II = Silicified (ver `data/instancia_20bloques.txt` y detalle abajo).
- [ ] Evaluar si conviene ampliar la instancia de prueba más allá de 20 bloques para mostrar escalabilidad (el Excel completo tiene 13.392 bloques disponibles).

## Prompt para usar IA en esta carpeta

Si algún integrante del equipo quiere usar un asistente de IA (Claude, ChatGPT, Copilot, etc.) para trabajar en este repositorio, puede empezar pegando el siguiente mensaje para darle contexto:

```
Estoy trabajando en el proyecto de Diseño y Análisis de Algoritmos (Tema 2, UCN,
primer semestre 2026) sobre optimización de procesamiento mineral con modos
operacionales. Es una variante de la mochila fraccionaria: hay que decidir cuánta
masa de cada bloque procesar bajo cada modo operacional de planta, maximizando el
valor económico total, sujeto a tiempo de planta disponible, masa máxima por
bloque, proporciones de mezcla entre tipos de roca por modo, y no negatividad.

Contexto de este repositorio:
- El enunciado y el marco general del proyecto están en los PDF de la carpeta raíz.
- El paper de referencia es minerals-15-00427-v2.pdf (Leiva, Lespay, Quelopana y
  Navarra, 2025, Minerals 15(4), 427). Describe su propia formulación y su propio
  matheuristic, pero nuestro grupo diseñó una propuesta algorítmica propia,
  distinta a la del paper (el enunciado no obliga a reproducir el método del
  paper).
- PropuestaAlgoritmica_Entrega2.docx documenta en lenguaje simple la idea del
  algoritmo, un ejemplo numérico paso a paso, el pseudocódigo, el análisis de
  complejidad y el argumento de validez de nuestra propuesta principal. Léelo
  antes de proponer cambios al diseño del algoritmo.
- cpp/ contiene la implementación en C++17 sin librerías externas: Instance
  (lectura de instancias), Baseline (línea base greedy de la Entrega 1),
  MainAlgorithm (propuesta principal: construcción greedy con reparto
  proporcional entre modos operacionales), Verifier (chequeo de factibilidad),
  main.cpp (corre y compara ambos sobre la misma instancia).
- README.md (este archivo) tiene las instrucciones de compilación, el detalle
  de la instancia de datos y la lista de lo que falta por hacer.

Por favor:
- No rediseñes el algoritmo principal ni la línea base sin revisar primero
  PropuestaAlgoritmica_Entrega2.docx: ya están decididos y justificados ahí.
- Si modificas el código, mantén el estilo existente (sin dependencias externas,
  comentarios solo donde el porqué no es obvio a simple vista).
- Antes de asumir qué falta por hacer, revisa la sección correspondiente de
  este README.md.
```

## Origen y supuestos de la instancia de datos (`data/instancia_20bloques.txt`)

- **Bloques** (tipo de roca, valor $v_{bo}$ por modo): extraídos de `DatosTema2.xlsx` (entregado por el profesor), tomando un vecindario de 20 bloques cercano al centroide de los bloques con valor positivo, en vez de las primeras 20 filas literales del archivo. Se optó por esto porque esas primeras filas corresponden a la capa superficial del modelo de bloques, enteramente estéril (mismo valor negativo repetido en las 20), lo que habría dado una instancia trivial (óptimo = no procesar nada).
- **Masa por bloque** ($m_b = 15.375$ ton), **tasas de procesamiento** ($r_A = 250$, $r_B = 200$ ton/h) y **fracciones de mezcla** ($w_A = 65\%/35\%$, $w_B = 45\%/55\%$): tomados de la Tabla 2 del paper de referencia, que describe el mismo caso de estudio de 13.392 bloques que corresponde a este Excel.
- **Tiempo de planta** ($q$): el paper reporta "Infinito" para este caso de estudio. Como valor finito se usó $q = 1.000.000$ h — lo bastante grande para que el tiempo nunca sea la restricción activa (igual que en el paper), sin dejar de ser representable. El problema sigue sin ser trivial: la masa por bloque y la proporción de mezcla exigida siguen acotando la solución (en esta instancia hay solo 5 bloques de tipo II frente a 15 de tipo I, por lo que el tipo II es el recurso escaso que limita cuánto se puede procesar en total).
- **Supuesto pendiente de confirmar**: el Excel no indica cuál tipo de roca ("I" o "II") corresponde a "Diorite porphyry" y cuál a "Silicified intrusive breccia" en la Tabla 2 del paper. Se asumió I = Diorite porphyry, II = Silicified intrusive breccia. Esto no afecta la validez del algoritmo ni de los resultados numéricos, solo la interpretación geológica en el informe.

## Resultados preliminares verificados

Sobre `data/instancia_20bloques.txt`, compilando con GCC 16.1.0 (mismo compilador que instalaron en el equipo), tanto en `-O0` como en `-O2`:

| | Línea base (Entrega 1) | Propuesta principal (Entrega 2) |
|---|---|---|
| Valor total | 0 | ≈ 87.147.774 |
| Factibilidad | Válida | Válida |

El valor 0 de la línea base **no es un error**: es la limitación que ya se documentó en `Baseline.hpp`. Con esta instancia (solo 5 bloques de tipo II frente a 15 de tipo I), el primer bloque que asigna la línea base agota todo lo que puede de un tipo antes de tocar el otro, y al corregir la mezcla ese primer intento queda escalado a cero — y esa masa nunca se recupera. Es exactamente el escenario que motivó a preferir la propuesta principal, y es un buen ejemplo concreto para la sección de "identificación de dificultades" de la Entrega 2.
