#pragma once

#include <string>

using namespace std;

class Date {
public:
    Date();
    Date(int year, int month, int day);

    int getYear() const;
    int getMonth() const;
    int getDay() const;

    bool equals(const Date& other) const;
    string toString() const;

private:
    int year;
    int month;
    int day;
    
};
