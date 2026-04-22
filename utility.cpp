#include "utility.h"
#include <sstream>

int StringUtil::split(const std::string& str, const std::string& delimiter,
                     std::string* parts, int maxParts) {
    int count = 0;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos && count < maxParts - 1) {
        parts[count++] = str.substr(start, end - start);
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    if (count < maxParts) {
        parts[count++] = str.substr(start);
    }

    return count;
}

std::string StringUtil::trim(const std::string& str) {
    size_t start = 0;
    size_t end = str.length();

    while (start < end && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n')) {
        start++;
    }

    while (end > start && (str[end - 1] == ' ' || str[end - 1] == '\t' || str[end - 1] == '\n')) {
        end--;
    }

    return str.substr(start, end - start);
}

bool StringUtil::isChineseChar(const std::string& str, int pos) {
    if (pos >= str.length() - 1) return false;
    unsigned char c1 = str[pos];
    unsigned char c2 = str[pos + 1];
    return (c1 >= 0xB0 && c1 <= 0xF7) && (c2 >= 0xA1 && c2 <= 0xFE);
}

int StringUtil::parseInt(const std::string& str) {
    int result = 0;
    int sign = 1;
    size_t i = 0;

    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }

    for (; i < str.length(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        } else {
            break;
        }
    }

    return result * sign;
}

std::string StringUtil::toString(int value) {
    if (value == 0) return "0";

    std::string result;
    bool negative = value < 0;
    if (negative) value = -value;

    while (value > 0) {
        result = char('0' + value % 10) + result;
        value /= 10;
    }

    if (negative) result = "-" + result;
    return result;
}

bool DateUtil::isValidDate(const std::string& date) {
    if (date.length() != 5 || date[2] != '-') return false;

    int month = StringUtil::parseInt(date.substr(0, 2));
    int day = StringUtil::parseInt(date.substr(3, 2));

    if (month < 1 || month > 12) return false;
    if (day < 1 || day > daysInMonth(month, 2021)) return false;

    return true;
}

int DateUtil::compareDates(const std::string& date1, const std::string& date2) {
    int m1 = StringUtil::parseInt(date1.substr(0, 2));
    int d1 = StringUtil::parseInt(date1.substr(3, 2));
    int m2 = StringUtil::parseInt(date2.substr(0, 2));
    int d2 = StringUtil::parseInt(date2.substr(3, 2));

    if (m1 != m2) return m1 - m2;
    return d1 - d2;
}

int DateUtil::daysInMonth(int month, int year) {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return 28;
        default:
            return 0;
    }
}

int DateUtil::dateToDays(const std::string& date) {
    int month = StringUtil::parseInt(date.substr(0, 2));
    int day = StringUtil::parseInt(date.substr(3, 2));

    int days = 0;
    for (int i = 1; i < month; i++) {
        days += daysInMonth(i, 2021);
    }
    days += day - 1;

    return days;
}

std::string DateUtil::daysToDate(int days) {
    int month = 1;
    while (days >= daysInMonth(month, 2021)) {
        days -= daysInMonth(month, 2021);
        month++;
    }

    std::string result;
    if (month < 10) result += "0";
    result += std::to_string(month) + "-";
    if (days + 1 < 10) result += "0";
    result += std::to_string(days + 1);

    return result;
}

bool TimeUtil::isValidTime(const std::string& time) {
    if (time.length() != 5 || time[2] != ':') return false;

    int hour = StringUtil::parseInt(time.substr(0, 2));
    int min = StringUtil::parseInt(time.substr(3, 2));

    if (hour < 0 || hour >= 24) return false;
    if (min < 0 || min >= 60) return false;

    return true;
}

int TimeUtil::timeToMinutes(const std::string& time) {
    int hour = StringUtil::parseInt(time.substr(0, 2));
    int min = StringUtil::parseInt(time.substr(3, 2));
    return hour * 60 + min;
}

std::string TimeUtil::minutesToTime(int minutes) {
    int hour = minutes / 60;
    int min = minutes % 60;

    std::string result;
    if (hour < 10) result += "0";
    result += std::to_string(hour) + ":";
    if (min < 10) result += "0";
    result += std::to_string(min);

    return result;
}

int TimeUtil::addMinutes(const std::string& time, int minutes) {
    int totalMinutes = timeToMinutes(time) + minutes;
    return totalMinutes;
}