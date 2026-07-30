#include "PmergeMe.hpp"

#include <ctime>
#include <exception>
#include <iomanip>
#include <iostream>

void printVector(const std::string& label,const std::vector<int>& values)
{
    std::cout << label;

    for (std::vector<int>::const_iterator it = values.begin();
         it != values.end();
         ++it)
    {
        std::cout << " " << *it;
    }
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    try
    {
        PmergeMe sorter;

        sorter.validateArguments(argc, argv);

        sorter.loadVector(argc, argv);

        printVector("Before:",sorter.getVector());

        const std::clock_t vectorStart = std::clock();

        sorter.sortVector();

        const std::clock_t vectorEnd =std::clock();

        const std::clock_t dequeStart =std::clock();

        sorter.loadDeque(argc, argv);
        sorter.sortDeque();

        const std::clock_t dequeEnd = std::clock();

        printVector("After:",sorter.getVector());

        const double vectorTime =static_cast<double>(vectorEnd - vectorStart)
            * 1000000.0
            / CLOCKS_PER_SEC;

        const double dequeTime =static_cast<double>(dequeEnd - dequeStart)
            * 1000000.0
            / CLOCKS_PER_SEC;

        std::cout
            << "Time to process a range of "
            << sorter.getVector().size()
            << " elements with std::vector : "
            << std::fixed
            << std::setprecision(5)
            << vectorTime
            << " us"
            << std::endl;

        std::cout
            << "Time to process a range of "
            << sorter.getDeque().size()
            << " elements with std::deque  : "
            << std::fixed
            << std::setprecision(5)
            << dequeTime
            << " us"
            << std::endl;
    }
    catch (const std::exception& exception)
    {
        std::cerr
            << exception.what()
            << std::endl;

        return 1;
    }

    return 0;
}