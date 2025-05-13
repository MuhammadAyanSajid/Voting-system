#include "DateTime.h"
#include <string>

using namespace std;

string DateTime::padZero(int value) const {
    if (value < 10) {
        return "0" + to_string(value);
    }
    return to_string(value);
}

bool DateTime::validateDateTime(int year, int month, int day, int hour, int minute, int second) {
    if (year < 1900 || year > 3000) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    if (hour < 0 || hour > 23) return false;
    if (minute < 0 || minute > 59) return false;
    if (second < 0 || second > 59) return false;

    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (isLeap && day > 29) return false;
        if (!isLeap && day > 28) return false;
    }

    return true;
}


DateTime::DateTime() : isValid(true) {
    GetLocalTime(&st);
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second) {
    isValid = validateDateTime(year, month, day, hour, minute, second);
    if (isValid) {
        st.wYear = (WORD)year;
        st.wMonth = (WORD)month;
        st.wDay = (WORD)day;
        st.wHour = (WORD)hour;
        st.wMinute = (WORD)minute;
        st.wSecond = (WORD)second;
        st.wMilliseconds = 0;
        st.wDayOfWeek = 0;
    }
    else {
        st.wYear = 0; st.wMonth = 0; st.wDay = 0; st.wHour = 0;
        st.wMinute = 0; st.wSecond = 0; st.wMilliseconds = 0; st.wDayOfWeek = 0;
    }
}

DateTime::DateTime(const DateTime& other) : st(other.st), isValid(other.isValid) {}

DateTime& DateTime::operator=(const DateTime& other) {
    if (this != &other) {
        st = other.st;
        isValid = other.isValid;
    }
    return *this;
}

int DateTime::getYear() const { return isValid ? (int)st.wYear : 0; }
int DateTime::getMonth() const { return isValid ? (int)st.wMonth : 0; }
int DateTime::getDay() const { return isValid ? (int)st.wDay : 0; }
int DateTime::getHour() const { return isValid ? (int)st.wHour : 0; }
int DateTime::getMinute() const { return isValid ? (int)st.wMinute : 0; }
int DateTime::getSecond() const { return isValid ? (int)st.wSecond : 0; }
bool DateTime::isObjectValid() const { return isValid; }

string DateTime::toString() const {
    if (!isValid) {
        return "Invalid DateTime";
    }
    return getDateString() + " " + getTimeString();
}

string DateTime::getDateString() const {
    if (!isValid) {
        return "Invalid Date";
    }
    return to_string(st.wYear) + "-" + padZero(st.wMonth) + "-" + padZero(st.wDay);
}

string DateTime::getTimeString() const {
    if (!isValid) {
        return "Invalid Time";
    }
    return padZero(st.wHour) + ":" + padZero(st.wMinute) + ":" + padZero(st.wSecond);
}


bool DateTime::operator==(const DateTime& other) const {
    if (!isValid || !other.isValid) return false;
    return st.wYear == other.st.wYear &&
        st.wMonth == other.st.wMonth &&
        st.wDay == other.st.wDay &&
        st.wHour == other.st.wHour &&
        st.wMinute == other.st.wMinute &&
        st.wSecond == other.st.wSecond;
}

bool DateTime::operator!=(const DateTime& other) const {
    return !(*this == other);
}

bool DateTime::operator<(const DateTime& other) const {
    if (!isValid || !other.isValid) return false;
    if (st.wYear != other.st.wYear) return st.wYear < other.st.wYear;
    if (st.wMonth != other.st.wMonth) return st.wMonth < other.st.wMonth;
    if (st.wDay != other.st.wDay) return st.wDay < other.st.wDay;
    if (st.wHour != other.st.wHour) return st.wHour < other.st.wHour;
    if (st.wMinute != other.st.wMinute) return st.wMinute < other.st.wMinute;
    return st.wSecond < other.st.wSecond;
}

bool DateTime::operator>(const DateTime& other) const {
    if (!isValid || !other.isValid) return false;
    return other < *this;
}

bool DateTime::operator<=(const DateTime& other) const {
    if (!isValid || !other.isValid) return false;
    return !(other < *this);
}

bool DateTime::operator>=(const DateTime& other) const {
    if (!isValid || !other.isValid) return false;
    return !(*this < other);
}

DateTime DateTime::getCurrentDateTime() {
    return DateTime();
}
