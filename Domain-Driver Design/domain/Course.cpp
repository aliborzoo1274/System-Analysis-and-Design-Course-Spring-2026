#include "Course.hpp"

using namespace std;

Course::Course() : id(0) {}

Course::Course(int id, const string& code, const string& title, int numOfUnits, const vector<int>& prerequisiteCourseIds)
    : id(id), code(code), title(title), numOfUnits(numOfUnits), prerequisiteCourseIds(prerequisiteCourseIds) {}

int Course::getId() const {
    return id;
}

string Course::getCode() const {
    return code;
}

string Course::getTitle() const {
    return title;
}

int Course::getNumOfUnits() const {
    return numOfUnits;
}

const vector<int>& Course::getPrerequisiteCourseIds() const {
    return prerequisiteCourseIds;
}
