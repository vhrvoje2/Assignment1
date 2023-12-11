CXX := g++
OUTPUT := sfmlgame
OUTPUT_DIR := ./bin

SFML_DIR := .

CXX_FLAGS := -O3 -std=c++2a -Wno-unused-result
INCLUDES := -I./src -I ./src/imgui -I$(SFML_DIR)/include
LDFLAGS := -O3 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL -L$(SFML_DIR)/lib

SRC_FILES := $(wildcard src/*.cpp src/imgui/*.cpp)
OBJ_FILES := $(patsubst %.cpp,%.o,$(SRC_FILES))

all: $(OUTPUT)

$(OUTPUT): | $(OUTPUT_DIR) $(OBJ_FILES) Makefile
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o $(OUTPUT_DIR)/$@

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

%.o: %.cpp Makefile
	$(CXX) -c $(CXX_FLAGS) $(INCLUDES) $< -o $@

clean:
	rm -f $(OBJ_FILES) $(OUTPUT_DIR)/$(OUTPUT)

run: $(OUTPUT)
	cd $(OUTPUT_DIR) && ./$(OUTPUT) && cd ..
