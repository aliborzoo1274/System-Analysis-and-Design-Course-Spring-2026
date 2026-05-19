#include "ExamTime.hpp"

#include <sstream>

using namespace std;

ExamTime::ExamTime() : date(Date()), start(Time()), end(Time()) {}

ExamTime::ExamTime(const Date& date, const Time& start, const Time& end)
    : date(date), start(start), end(end) {}

const Date& ExamTime::getDate() const {
    return date;
}

const Time& ExamTime::getStart() const {
    return start;
}

const Time& ExamTime::getEnd() const {
    return end;
}

bool ExamTime::overlaps(const ExamTime& other) const {
    if (!date.equals(other.date)) {
        return false;
    }

    return Time::isOverlap(start, end, other.start, other.end);
}

string ExamTime::toString() const {
    stringstream ss;
    ss << date.toString() << " " << start.toString() << "-" << end.toString();
    return ss.str();
}
