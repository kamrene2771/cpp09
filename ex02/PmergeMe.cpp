#include "PmergeMe.hpp"
#include <stdexcept>

PmergeMe::PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe& other)
    : _vector(other._vector),
      _deque(other._deque)
{
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
    if (this != &other)
    {
        _vector = other._vector;
        _deque = other._deque;
    }

    return *this;
}

PmergeMe::~PmergeMe()
{
}

int PmergeMe::parsePositiveInteger(
    const std::string& text) const
{
    if (text.empty())
        throw std::runtime_error("Error");

    long value = 0;

    for (std::string::size_type i = 0;
         i < text.length();
         ++i)
    {
        if (text[i] < '0' || text[i] > '9')
            throw std::runtime_error("Error");

        value = value * 10 + (text[i] - '0');

        if (value > INT_MAX)
            throw std::runtime_error("Error");
    }

    if (value <= 0)
        throw std::runtime_error("Error");

    return static_cast<int>(value);
}

void PmergeMe::checkDuplicate(
    const std::vector<int>& values,
    int value) const
{
    for (std::vector<int>::const_iterator it = values.begin();
         it != values.end();
         ++it)
    {
        if (*it == value)
            throw std::runtime_error("Error");
    }
}

void PmergeMe::validateArguments(int argc,char** argv) const
{
    if (argc < 2)
        throw std::runtime_error("Error");

    std::vector<int> values;

    for (int i = 1; i < argc; ++i)
    {
        const int value = parsePositiveInteger(argv[i]);
        checkDuplicate(values, value);
        values.push_back(value);
    }
}

void PmergeMe::loadVector(int argc, char** argv)
{
    for (int i = 1; i < argc; ++i)
        _vector.push_back(parsePositiveInteger(argv[i]));
}

void PmergeMe::loadDeque(int argc, char** argv)
{
    for (int i = 1; i < argc; ++i)
        _deque.push_back(parsePositiveInteger(argv[i]));
}

std::vector<std::size_t>PmergeMe::buildVectorInsertionOrder(std::size_t lastIndex) const
{
    std::vector<std::size_t> order;

    if (lastIndex < 2)
        return order;

    std::size_t previousJacobsthal = 1;
    std::size_t jacobsthal = 3;
    std::size_t previousBoundary = 1;

    while (jacobsthal <= lastIndex)
    {
        for (std::size_t index = jacobsthal; index > previousBoundary;--index)
        {
            order.push_back(index);
        }

        previousBoundary = jacobsthal;

        const std::size_t nextJacobsthal = jacobsthal + 2 * previousJacobsthal;

        previousJacobsthal = jacobsthal;
        jacobsthal = nextJacobsthal;
    }

    for (std::size_t index = lastIndex;index > previousBoundary; --index)//last index
    {
        order.push_back(index);
    }

    return order;
}

std::deque<std::size_t>PmergeMe::buildDequeInsertionOrder(std::size_t lastIndex) const
{
    std::deque<std::size_t> order;

    if (lastIndex < 2)
        return order;

    std::size_t previousJacobsthal = 1;
    std::size_t jacobsthal = 3;
    std::size_t previousBoundary = 1;

    while (jacobsthal <= lastIndex)
    {
        for (std::size_t index = jacobsthal; index > previousBoundary; --index)
        {
            order.push_back(index);
        }

        previousBoundary = jacobsthal;

        const std::size_t nextJacobsthal =jacobsthal + 2 * previousJacobsthal;

        previousJacobsthal = jacobsthal;
        jacobsthal = nextJacobsthal;
    }

    for (std::size_t index = lastIndex; index > previousBoundary; --index)
    {
        order.push_back(index);
    }

    return order;
}

void PmergeMe::insertVectorValue(std::vector<int>& chain,int value,int partner) const
{
    std::size_t right = chain.size();

    if (partner >= 0)
    {
        for (std::size_t i = 0; i < chain.size(); ++i)
        {
            if (chain[i] == partner)
            {
                right = i;
                break;
            }
        }
    }

    std::size_t left = 0;

    while (left < right)
    {
        const std::size_t middle = left + (right - left) / 2;

        if (chain[middle] < value)
            left = middle + 1;
        else
            right = middle;
    }

    chain.insert(chain.begin()+ static_cast<std::vector<int>::difference_type>(left),value);
}

void PmergeMe::insertDequeValue(std::deque<int>& chain,int value,int partner) const
{
    std::size_t right = chain.size();

    if (partner >= 0)
    {
        for (std::size_t i = 0; i < chain.size(); ++i)
        {
            if (chain[i] == partner)
            {
                right = i;
                break;
            }
        }
    }

    std::size_t left = 0;

    while (left < right)
    {
        const std::size_t middle =
            left + (right - left) / 2;

        if (chain[middle] < value)
            left = middle + 1;
        else
            right = middle;
    }

    chain.insert(chain.begin()+ static_cast<std::deque<int>::difference_type>(left),value);
}

void PmergeMe::sortVector(std::vector<int>& values) const
{
    if (values.size() <= 1)
        return;

    const bool hasOddValue = values.size() % 2 != 0;
    const int oddValue = hasOddValue ? values.back() : -1;
    const std::size_t pairCount = values.size() / 2;

    std::vector<int> smaller;
    std::vector<int> larger;

    for (std::size_t i = 0;i + 1 < values.size();i += 2)
    {
        if (values[i] < values[i + 1])
        {
            smaller.push_back(values[i]);
            larger.push_back(values[i + 1]);
        }
        else
        {
            smaller.push_back(values[i + 1]);
            larger.push_back(values[i]);
        }
    }

    std::vector<int> sortedLarger = larger;
    sortVector(sortedLarger);
    std::vector<int> orderedSmaller;

    for (std::size_t i = 0;i < sortedLarger.size();++i)
    {
        for (std::size_t j = 0;j < larger.size();++j)
        {
            if (larger[j] == sortedLarger[i])
            {
                orderedSmaller.push_back(smaller[j]);
                break;
            }
        }
    }

    std::vector<int> chain;
    chain.push_back(orderedSmaller[0]);

    for (std::size_t i = 0;i < sortedLarger.size(); ++i)
    {
        chain.push_back(sortedLarger[i]);
    }

    const std::size_t lastPendingIndex = pairCount + (hasOddValue ? 1 : 0);

    const std::vector<std::size_t> order = buildVectorInsertionOrder(lastPendingIndex);

    for (std::size_t i = 0;i < order.size();++i)
    {
        const std::size_t pairIndex = order[i];
        if (pairIndex <= pairCount)
        {
            insertVectorValue(chain, orderedSmaller[pairIndex - 1], sortedLarger[pairIndex - 1]);
        }
        else
        {
            insertVectorValue(chain, oddValue, -1);
        }
    }

    values = chain;
}

void PmergeMe::sortDeque(std::deque<int>& values) const
{
    if (values.size() <= 1)
        return;

    const bool hasOddValue = values.size() % 2 != 0;
    const int oddValue = hasOddValue ? values.back() : -1;
    const std::size_t pairCount = values.size() / 2;

    std::deque<int> smaller;
    std::deque<int> larger;

    for (std::size_t i = 0;i + 1 < values.size();i += 2)
    {
        if (values[i] < values[i + 1])
        {
            smaller.push_back(values[i]);
            larger.push_back(values[i + 1]);
        }
        else
        {
            smaller.push_back(values[i + 1]);
            larger.push_back(values[i]);
        }
    }

    std::deque<int> sortedLarger = larger;

    sortDeque(sortedLarger);

    std::deque<int> orderedSmaller;

    for (std::size_t i = 0; i < sortedLarger.size(); ++i)
    {
        for (std::size_t j = 0; j < larger.size();++j)
        {
            if (larger[j] == sortedLarger[i])
            {
                orderedSmaller.push_back(smaller[j]);
                break;
            }
        }
    }

    std::deque<int> chain;

    chain.push_back(orderedSmaller[0]);

    for (std::size_t i = 0;i < sortedLarger.size();++i)
    {
        chain.push_back(sortedLarger[i]);
    }

    const std::size_t lastPendingIndex = pairCount + (hasOddValue ? 1 : 0);
    const std::deque<std::size_t> order = buildDequeInsertionOrder(lastPendingIndex);

    for (std::size_t i = 0; i < order.size(); ++i)
    {
        const std::size_t pairIndex = order[i];

        if (pairIndex <= pairCount)
        {
            insertDequeValue(chain, orderedSmaller[pairIndex - 1], sortedLarger[pairIndex - 1]);
        }
        else
        {
            insertDequeValue(chain, oddValue, -1);
        }
    }
    values = chain;
}

void PmergeMe::sortVector()
{
    sortVector(_vector);
}

void PmergeMe::sortDeque()
{
    sortDeque(_deque);
}

const std::vector<int>&PmergeMe::getVector() const
{
    return _vector;
}

const std::deque<int>&PmergeMe::getDeque() const
{
    return _deque;
}
