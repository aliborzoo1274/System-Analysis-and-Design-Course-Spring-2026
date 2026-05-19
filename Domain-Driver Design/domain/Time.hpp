#pragma once

#include <string>

using namespace std;

class Time {
public:
    Time();
    Time(int hour, int minute);

    int getHour() const;
    int getMinute() const;
    int toMinutes() const;
    string toString() const;

    static bool isOverlap(const Time& start1, const Time& end1, const Time& start2, const Time& end2);

private:
    int hour;
    int minute;

};
