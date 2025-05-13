#ifndef DATETIME_H
#define DATETIME_H

#include <windows.h>
#include <string>
#include <iostream>

using namespace std;

class DateTime {
private:
    SYSTEMTIME st;
    bool isValid;

    string padZero(int value) const;
    bool validateDateTime(int year, int month, int day, int hour, int minute, int second);

public:
    DateTime();

    DateTime(int year, int month, int day, int hour, int minute, int second);

    DateTime(const DateTime& other);

    DateTime& operator=(const DateTime& other);

    ~DateTime() = default;

    int getYear() const;
    int getMonth() const;
    int getDay() const;
    int getHour() const;
    int getMinute() const;
    int getSecond() const;
    bool isObjectValid() const;

    string toString() const;
    string getDateString() const;
    string getTimeString() const;

    bool operator==(const DateTime& other) const;
    bool operator!=(const DateTime& other) const;
    bool operator<(const DateTime& other) const;
    bool operator>(const DateTime& other) const;
    bool operator<=(const DateTime& other) const;
    bool operator>=(const DateTime& other) const;

    static DateTime getCurrentDateTime();
};

#endif