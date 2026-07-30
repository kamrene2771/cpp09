#ifndef RPN_HPP
#define RPN_HPP

#include <string>

class RPN
{
private:
    bool isOperator(char token) const;
    int calculate(int left, int right, char operation) const;

public:
    RPN();
    RPN(const RPN& other);
    RPN& operator=(const RPN& other);
    ~RPN();

    int evaluate(const std::string& expression) const;
};

#endif