OUT = MazeWars

CC := g++

OUTPUT_DIR := ./output
SRC_DIR := ./src
UTILS_DIR := utils

OBJS := $(OUTPUT_DIR)/main.o $(OUTPUT_DIR)/game.o $(OUTPUT_DIR)/draw.o $(OUTPUT_DIR)/maze.o $(OUTPUT_DIR)/client.o $(OUTPUT_DIR)/server.o $(OUTPUT_DIR)/helper.o $(OUTPUT_DIR)/start.o $(OUTPUT_DIR)/gui.o $(OUTPUT_DIR)/character.o

COMMON_FLAGS := `pkg-config --cflags --libs sdl2` -pthread -lSDL2_ttf -lSDL2_image

# output
build: $(OBJS)
	$(CC) -o $(OUT) $^ $(COMMON_FLAGS)

$(OUTPUT_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CC) $< -c -o $@ $(COMMON_FLAGS)

# src -> cpp
$(OUTPUT_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $< -c -o $@ $(COMMON_FLAGS)

# src/utils -> cpp
$(OUTPUT_DIR)/%.o: $(SRC_DIR)/$(UTILS_DIR)/%.cpp
	$(CC) $< -c -o $@ $(COMMON_FLAGS)

# libsdl2-dev


