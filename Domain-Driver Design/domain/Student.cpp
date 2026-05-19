#include "Student.hpp"

using namespace std;

Student::Student() : id(0) {}

Student::Student(int id, const string& name) : id(id), name(name) {}

Student::Student(int id, const string& name, const vector<int>& completedCourseIds) : id(id), name(name), completedCourseIds(completedCourseIds) {}

int Student::getId() const {
    return id;
}

string Student::getName() const {
    return name;
}

bool Student::hasCompletedCourse(int courseId) const {
    return find(completedCourseIds.begin(), completedCourseIds.end(), courseId) != completedCourseIds.end();
}

bool Student::hasCompletedPrerequisites(const vector<int>& prerequisites) {
    for (auto courseId : completedCourseIds) {
        if (!hasCompletedCourse(courseId)) {
            return false;
        }
    }
    return true;
}

void Student::addCompletedCourse(int courseId) {
    if (!hasCompletedCourse(courseId)) {
        completedCourseIds.push_back(courseId);
    }
}
