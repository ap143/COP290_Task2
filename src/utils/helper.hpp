#ifndef HELPER
#define HELPER

#include <iostream>

void print(std::string str);
void error(std::string message);

bool inRect(float x, float y, float rx, float ry, float width, float height);

void sendMessage(std::string message);
void respond(std::string message);

#endif