#include "Time.hpp"

#include <iomanip>
#include <sstream>
#include <stdexcept>

using namespace std;

Time::Time() : hour(0), minute(0) {}

Time::Time(int hour, int minute) : hour(hour), minute(minute) {
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        throw runtime_error("Invalid time.");
    }
}

int Time::getHour() const {
    return hour;
}

int Time::getMinute() const {
    return minute;
}

int Time::toMinutes() const {
    return hour * 60 + minute;
}

string Time::toString() const {
    stringstream ss;
    ss << setw(2) << setfill('0') << hour << ":" << setw(2) << setfill('0') << minute;
    return ss.str();
}

bool Time::isOverlap(const Time& start1, const Time& end1, const Time& start2, const Time& end2) {
    int aStart = start1.toMinutes();
    int aEnd = end1.toMinutes();
    int bStart = start2.toMinutes();
    int bEnd = end2.toMinutes();

    if (aEnd <= aStart || bEnd <= bStart) {
        throw runtime_error("End time must be after start time.");
    }

    return aStart < bEnd && bStart < aEnd;
}
