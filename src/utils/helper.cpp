#include "helper.hpp"

void print(std::string str)
{
    std::cout << str << std::endl;
}

void error(std::string message)
{
    std::cerr << message << std::endl;
    exit(-1);
}

bool inRect(float x, float y, float rx, float ry, float width, float height)
{
    return x > rx && x < rx + width && y > ry && y < ry + height;
}
