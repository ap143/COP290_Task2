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
