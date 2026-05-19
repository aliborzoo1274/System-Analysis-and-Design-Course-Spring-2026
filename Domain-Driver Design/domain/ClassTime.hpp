#pragma once

#include "Time.hpp"

#include <string>
#include <vector>

using namespace std;

enum class DayOfWeek {
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};

class ClassTime {
public:
    ClassTime();
    ClassTime(const vector<DayOfWeek>& days, const Time& start, const Time& end);

    const vector<DayOfWeek>& getDays() const;
    const Time& getStart() const;
    const Time& getEnd() const;

    bool overlaps(const ClassTime& other) const;
    string toString() const;

private:
    vector<DayOfWeek> days;
    Time start;
    Time end;
    
    bool hasDay(DayOfWeek day) const;
    static string dayToString(DayOfWeek day);
};
