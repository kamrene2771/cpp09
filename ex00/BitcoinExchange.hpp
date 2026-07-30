#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

class BitcoinExchange
{
private:
    std::map<std::string, double> _rates;

    int parseTwoDigits(const std::string& text,std::string::size_type position) const;
    int parseFourDigits(const std::string& text,std::string::size_type position) const;
    void printBadInput(const std::string& line) const;

    bool isValidDate(const std::string& date) const;
    bool parseNumber(const std::string& text, double& value) const;
    double findRate(const std::string& date) const;

public:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& other);
    BitcoinExchange& operator=(const BitcoinExchange& other);
    ~BitcoinExchange();

    void loadDatabase(const std::string& filename);
    void processInputFile(const std::string& filename) const;
};

#endif