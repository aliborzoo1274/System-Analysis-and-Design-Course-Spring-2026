#include "AcademicRecord.hpp"

#include <algorithm>

using namespace std;

AcademicRecord::AcademicRecord() {}

AcademicRecord::AcademicRecord(const vector<int>& completedCourseIds)
    : completedCourseIds(completedCourseIds) {}

const vector<int>& AcademicRecord::getCompletedCourseIds() const {
    return completedCourseIds;
}

bool AcademicRecord::hasCompletedCourse(int courseId) const {
    return find(completedCourseIds.begin(), completedCourseIds.end(), courseId) != completedCourseIds.end();
}

void AcademicRecord::addCompletedCourse(int courseId) {
    if (!hasCompletedCourse(courseId)) {
        completedCourseIds.push_back(courseId);
    }
}
