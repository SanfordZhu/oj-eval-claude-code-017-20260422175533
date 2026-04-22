#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <cstring>

class StringUtil {
public:
    static int split(const std::string& str, const std::string& delimiter,
                     std::string* parts, int maxParts);

    static std::string trim(const std::string& str);

    static bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    static bool isLetter(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    static bool isChineseChar(const std::string& str, int pos);

    static int parseInt(const std::string& str);

    static std::string toString(int value);
};

class DateUtil {
public:
    static bool isValidDate(const std::string& date);
    static int compareDates(const std::string& date1, const std::string& date2);
    static int daysInMonth(int month, int year);
    static int dateToDays(const std::string& date);
    static std::string daysToDate(int days);
};

class TimeUtil {
public:
    static bool isValidTime(const std::string& time);
    static int timeToMinutes(const std::string& time);
    static std::string minutesToTime(int minutes);
    static int addMinutes(const std::string& time, int minutes);
};

#endif