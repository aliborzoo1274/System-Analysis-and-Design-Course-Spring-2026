#include "ClassTime.hpp"

#include <sstream>

using namespace std;

ClassTime::ClassTime() : start(Time()), end(Time()) {}

ClassTime::ClassTime(const vector<DayOfWeek>& days, const Time& start, const Time& end)
    : days(days), start(start), end(end) {}

const vector<DayOfWeek>& ClassTime::getDays() const {
    return days;
}

const Time& ClassTime::getStart() const {
    return start;
}

const Time& ClassTime::getEnd() const {
    return end;
}

bool ClassTime::hasDay(DayOfWeek day) const {
    for (auto current : days) {
        if (current == day) {
            return true;
        }
    }
    return false;
}

bool ClassTime::overlaps(const ClassTime& other) const {
    for (auto day : days) {
        if (other.hasDay(day) && Time::isOverlap(start, end, other.start, other.end)) {
            return true;
        }
    }
    return false;
}

string ClassTime::dayToString(DayOfWeek day) {
    switch (day) {
        case DayOfWeek::Monday:
            return "Mon";
        case DayOfWeek::Tuesday:
            return "Tue";
        case DayOfWeek::Wednesday:
            return "Wed";
        case DayOfWeek::Thursday:
            return "Thu";
        case DayOfWeek::Friday:
            return "Fri";
        case DayOfWeek::Saturday:
            return "Sat";
        case DayOfWeek::Sunday:
            return "Sun";
        default:
            return "";
    }
}

string ClassTime::toString() const {
    stringstream ss;

    for (size_t i = 0; i < days.size(); ++i) {
        ss << dayToString(days[i]);
        if (i + 1 < days.size()) {
            ss << "/";
        }
    }

    ss << " " << start.toString() << "-" << end.toString();
    return ss.str();
}
