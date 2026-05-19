#include "Date.hpp"

#include <iomanip>
#include <sstream>
#include <stdexcept>

using namespace std;

Date::Date() : year(1970), month(1), day(1) {}

Date::Date(int year, int month, int day) : year(year), month(month), day(day) {
    if (month < 1 || month > 12 || day < 1 || day > 31) {
        throw runtime_error("Invalid date.");
    }
}

int Date::getYear() const {
    return year;
}

int Date::getMonth() const {
    return month;
}

int Date::getDay() const {
    return day;
}

bool Date::equals(const Date& other) const {
    return year == other.year && month == other.month && day == other.day;
}

string Date::toString() const {
    stringstream ss;
    ss << setw(4) << setfill('0') << year << "-" << setw(2) << setfill('0') << month << "-" << setw(2) << setfill('0') << day;
    return ss.str();
}
