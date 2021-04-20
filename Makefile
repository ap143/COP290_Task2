OUT = MazeWars

CC := g++

OUTPUT_DIR := ./output
SRC_DIR := ./src
UTILS_DIR := utils

OBJS := $(OUTPUT_DIR)/main.o $(OUTPUT_DIR)/game.o $(OUTPUT_DIR)/draw.o $(OUTPUT_DIR)/maze.o $(OUTPUT_DIR)/client.o $(OUTPUT_DIR)/server.o $(OUTPUT_DIR)/helper.o

COMMON_FLAGS := `pkg-config --cflags --libs sdl2` -lSDL2_ttf

# output
build: $(OBJS)
	$(CC) -o $(OUT) $^ $(COMMON_FLAGS)

# src -> cpp
$(OUTPUT_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $< -c -o $@ $(COMMON_FLAGS)

# src/utils -> cpp
$(OUTPUT_DIR)/%.o: $(SRC_DIR)/$(UTILS_DIR)/%.cpp
	$(CC) $< -c -o $@ $(COMMON_FLAGS)

