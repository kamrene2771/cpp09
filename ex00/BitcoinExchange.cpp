#include "BitcoinExchange.hpp"
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>



BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other)
    : _rates(other._rates)
{
}

BitcoinExchange& BitcoinExchange::operator=(
    const BitcoinExchange& other)
{
    if (this != &other)
        _rates = other._rates;

    return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

int BitcoinExchange::parseTwoDigits(const std::string& text,std::string::size_type position) const
{
    return (text[position] - '0') * 10 + (text[position + 1] - '0');
}

int BitcoinExchange::parseFourDigits(const std::string& text,std::string::size_type position) const
{
    return (text[position] - '0') * 1000 + (text[position + 1] - '0') * 100
        + (text[position + 2] - '0') * 10 + (text[position + 3] - '0');
}

void BitcoinExchange::printBadInput(const std::string& line) const
{
    std::cerr << "Error: bad input => " << line << std::endl;
}

bool BitcoinExchange::isValidDate(const std::string& date) const
{
    static const int daysInMonth[12] =
    {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    if (date.length() != 10)
        return false;

    if (date[4] != '-' || date[7] != '-')
        return false;

    for (std::string::size_type i = 0; i < date.length(); ++i)
    {
        if (i == 4 || i == 7)
            continue;

        if (!std::isdigit(static_cast<unsigned char>(date[i])))
            return false;
    }

    const int year = parseFourDigits(date, 0);
    const int month = parseTwoDigits(date, 5);
    const int day = parseTwoDigits(date, 8);

    if (month < 1 || month > 12)
        return false;

    int maximumDay = daysInMonth[month - 1];

    const bool leapYear =
        year % 400 == 0
        || (year % 4 == 0 && year % 100 != 0);

    if (month == 2 && leapYear)
        maximumDay = 29;

    return day >= 1 && day <= maximumDay;
}

bool BitcoinExchange::parseNumber(const std::string& text,double& value) const
{
    if (text.empty())
        return false;

    std::stringstream stream(text);

    stream >> value;

    if (stream.fail())
        return false;

    stream >> std::ws;

    return stream.eof();
}

double BitcoinExchange::findRate(const std::string& date) const
{
    if (_rates.empty())
        throw std::runtime_error("Error: empty database.");

    std::map<std::string, double>::const_iterator it;

    it = _rates.lower_bound(date);

    if (it != _rates.end() && it->first == date)
        return it->second;

    if (it == _rates.begin())
    {
        throw std::runtime_error(
            "Error: no exchange rate available for this date."
        );
    }

    --it;

    return it->second;
}

void BitcoinExchange::loadDatabase(const std::string& filename)
{
    std::ifstream file(filename.c_str());

    if (!file.is_open())
        throw std::runtime_error("Error: could not open database.");

    std::string line;

    if (!std::getline(file, line))
        throw std::runtime_error("Error: empty database.");

    if (line != "date,exchange_rate")
        throw std::runtime_error("Error: invalid database header.");

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        const std::string::size_type separator = line.find(',');

        if (separator == std::string::npos)
            continue;

        if (line.find(',', separator + 1) != std::string::npos)
            continue;

        const std::string date = line.substr(0, separator);
        const std::string rateText = line.substr(separator + 1);

        double rate;

        if (!isValidDate(date))
            continue;

        if (!parseNumber(rateText, rate))
            continue;

        if (rate < 0.0)
            continue;

        _rates[date] = rate;
    }

    if (_rates.empty())
        throw std::runtime_error("Error: empty database.");
}

void BitcoinExchange::processInputFile(const std::string& filename) const
{
    std::ifstream file(filename.c_str());

    if (!file.is_open())
        throw std::runtime_error("Error: could not open file.");

    std::string line;

    if (!std::getline(file, line))
        throw std::runtime_error("Error: empty input file.");

    if (line != "date | value")
        throw std::runtime_error("Error: invalid input header.");

    const std::string delimiter = " | ";

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        const std::string::size_type separator =
            line.find(delimiter);

        if (separator == std::string::npos)
        {
            printBadInput(line);
            continue;
        }

        if (line.find(delimiter,separator + delimiter.length()) != std::string::npos)
        {
            printBadInput(line);
            continue;
        }

        const std::string date = line.substr(0, separator);
        const std::string valueText = line.substr(separator + delimiter.length());

        double value;

        if (!isValidDate(date) || !parseNumber(valueText, value))
        {
            printBadInput(line);
            continue;
        }

        if (value < 0.0)
        {
            std::cerr
                << "Error: not a positive number."
                << std::endl;
            continue;
        }

        if (value > 1000.0)
        {
            std::cerr
                << "Error: too large a number."
                << std::endl;
            continue;
        }

        try
        {
            const double rate = findRate(date);

            std::cout
                << date
                << " => "
                << value
                << " = "
                << value * rate
                << std::endl;
        }
        catch (const std::exception& exception)
        {
            std::cerr << exception.what() << std::endl;
        }
    }
}