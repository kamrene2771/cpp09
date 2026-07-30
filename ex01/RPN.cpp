#include "RPN.hpp"

#include <cctype>
#include <stack>
#include <stdexcept>

RPN::RPN()
{
}

RPN::RPN(const RPN& other)
{
    (void)other;
}

RPN& RPN::operator=(const RPN& other)
{
    if (this != &other)
    {
    }
    return *this;
}

RPN::~RPN()
{
}

bool RPN::isOperator(char token) const
{
    return token == '+'
        || token == '-'
        || token == '*'
        || token == '/';
}

int RPN::calculate(int left, int right, char operation) const
{
    if (operation == '+')
        return left + right;

    if (operation == '-')
        return left - right;

    if (operation == '*')
        return left * right;

    if (operation == '/')
    {
        if (right == 0)
            throw std::runtime_error("Error: division by zero.");

        return left / right;
    }

    throw std::runtime_error("Error: invalid operator.");
}

int RPN::evaluate(const std::string& expression) const
{
    std::stack<int> numbers;

    for (std::string::size_type i = 0; i < expression.length(); ++i)
    {
        const char token = expression[i];

        if (std::isspace(static_cast<unsigned char>(token)))
            continue;

        if (std::isdigit(static_cast<unsigned char>(token)))
        {
            numbers.push(token - '0');
            continue;
        }

        if (!isOperator(token))
            throw std::runtime_error("Error: invalid token.");

        if (numbers.size() < 2)
            throw std::runtime_error("Error: invalid expression.");

        const int right = numbers.top();
        numbers.pop();

        const int left = numbers.top();
        numbers.pop();

        numbers.push(calculate(left, right, token));
    }

    if (numbers.size() != 1)
        throw std::runtime_error("Error: invalid expression.");

    return numbers.top();
}