build:
	g++ main.cpp game.cpp -o main `pkg-config --cflags --libs sdl2`