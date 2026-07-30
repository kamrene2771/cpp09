#include "RPN.hpp"

#include <exception>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Error: expected one RPN expression." << std::endl;
        return 1;
    }

    try
    {
        RPN calculator;

        std::cout << calculator.evaluate(argv[1]) << std::endl;
    }
    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << std::endl;
        return 1;
    }

    return 0;
}