CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Iinclude

SRC = src/Instance.cpp src/Baseline.cpp src/MainAlgorithm.cpp src/Verifier.cpp src/main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = procesamiento_mineral.exe

DATA_DIR = data
RESULTS_DIR = results

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crear carpeta de resultados
$(RESULTS_DIR):
	mkdir $(RESULTS_DIR) 2>nul

# Targets para cada instancia (definidos explícitamente)
run-20: $(TARGET) $(RESULTS_DIR)
	$(TARGET) $(DATA_DIR)/instancia_20bloques.txt > $(RESULTS_DIR)/resultados_20.txt

run-50: $(TARGET) $(RESULTS_DIR)
	$(TARGET) $(DATA_DIR)/instancia_50bloques.txt > $(RESULTS_DIR)/resultados_50.txt

run-100: $(TARGET) $(RESULTS_DIR)
	$(TARGET) $(DATA_DIR)/instancia_100bloques.txt > $(RESULTS_DIR)/resultados_100.txt

run-200: $(TARGET) $(RESULTS_DIR)
	$(TARGET) $(DATA_DIR)/instancia_200bloques.txt > $(RESULTS_DIR)/resultados_200.txt

run-500: $(TARGET) $(RESULTS_DIR)
	$(TARGET) $(DATA_DIR)/instancia_500bloques.txt > $(RESULTS_DIR)/resultados_500.txt

# Target para ejecutar todos
run-all: run-20 run-50 run-100 run-200 run-500
	@echo Todas las ejecuciones completadas. Resultados en $(RESULTS_DIR)/

clean:
	-del /Q $(OBJ) $(TARGET) 2>nul
	-rmdir /S /Q $(RESULTS_DIR) 2>nul

.PHONY: all clean run-all run-20 run-50 run-100 run-200 run-500