#pragma once

#include "Date.hpp"
#include "Time.hpp"

#include <string>

using namespace std;

class ExamTime {
public:
    ExamTime();
    ExamTime(const Date& date, const Time& start, const Time& end);

    const Date& getDate() const;
    const Time& getStart() const;
    const Time& getEnd() const;

    bool overlaps(const ExamTime& other) const;
    string toString() const;

private:
    Date date;
    Time start;
    Time end;

};
