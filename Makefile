OUT = MazeWars

CC := g++ -O3

OUTPUT_DIR := ./output
SRC_DIR := ./src
UTILS_DIR := utils

OBJS := $(OUTPUT_DIR)/main.o $(OUTPUT_DIR)/game.o $(OUTPUT_DIR)/draw.o $(OUTPUT_DIR)/maze.o $(OUTPUT_DIR)/client.o $(OUTPUT_DIR)/server.o $(OUTPUT_DIR)/helper.o  $(OUTPUT_DIR)/gui.o $(OUTPUT_DIR)/character.o $(OUTPUT_DIR)/teamView.o

COMMON_FLAGS := `pkg-config --cflags --libs sdl2` -lSDL2_ttf -lSDL2_image -Wall

# output
build: $(OBJS)
	$(CC) -o $(OUT) $^ $(COMMON_FLAGS)

$(OUTPUT_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CC) $< -c -o $@ $(COMMON_FLAGS)

# src -> cpp
$(OUTPUT_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.hpp
	$(CC) $< -c -o $@ $(COMMON_FLAGS)

# src/utils -> cpp
$(OUTPUT_DIR)/%.o: $(SRC_DIR)/$(UTILS_DIR)/%.cpp $(SRC_DIR)/$(UTILS_DIR)/%.hpp
	$(CC) $< -c -o $@ $(COMMON_FLAGS)

clean:
	$(RM) $(OUTPUT_DIR)/*.o

again:
	make clean && make

# libsdl2-dev


