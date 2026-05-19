#include "Course.hpp"

using namespace std;

Course::Course() : id(0) {}

Course::Course(int id, const string& code, const string& title, const vector<int>& prerequisiteCourseIds)
    : id(id), code(code), title(title), prerequisiteCourseIds(prerequisiteCourseIds) {}

int Course::getId() const {
    return id;
}

string Course::getCode() const {
    return code;
}

string Course::getTitle() const {
    return title;
}

const vector<int>& Course::getPrerequisiteCourseIds() const {
    return prerequisiteCourseIds;
}

bool Course::prerequisitesMet(const AcademicRecord& record) const {
    for (int courseId : prerequisiteCourseIds) {
        if (!record.hasCompletedCourse(courseId)) {
            return false;
        }
    }
    return true;
}
