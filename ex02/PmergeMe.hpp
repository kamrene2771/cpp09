#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <deque>
#include <string>
#include <vector>

class PmergeMe
{
private:
    std::vector<int> _vector;
    std::deque<int> _deque;

    int parsePositiveInteger(const std::string& text) const;
    void checkDuplicate(const std::vector<int>& values,int value) const;
    void sortVector(std::vector<int>& values) const;
    void sortDeque(std::deque<int>& values) const;

    void insertVectorValue(std::vector<int>& chain,int value,int partner) const;
    void insertDequeValue(std::deque<int>& chain,int value,int partner) const;

    std::vector<std::size_t> buildVectorInsertionOrder(std::size_t lastIndex) const;
    std::deque<std::size_t> buildDequeInsertionOrder(std::size_t lastIndex) const;

public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    void validateArguments(int argc, char** argv) const;

    void loadVector(int argc, char** argv);
    void loadDeque(int argc, char** argv);

    void sortVector();
    void sortDeque();

    const std::vector<int>& getVector() const;
    const std::deque<int>& getDeque() const;
};

#endif