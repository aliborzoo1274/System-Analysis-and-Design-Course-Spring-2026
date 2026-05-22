#include "AcademicRecord.hpp"

#include <algorithm>

using namespace std;

const float passingScore = 10;

AcademicRecord::AcademicRecord() {}

AcademicRecord::AcademicRecord(const unordered_map<int, float>& completedCoursesScore)
    : completedCoursesScore(completedCoursesScore) {}

bool AcademicRecord::hasCompletedCourse(int courseId) const {
    return completedCoursesScore.find(courseId) != completedCoursesScore.end();
}

bool AcademicRecord::hasCompletedPrerequisites(const vector<int>& prerequisites) {
    for (int prereqId : prerequisites) {
        if (!hasCompletedCourse(prereqId)) {
            return false;
        }
    }
    return true;
}

void AcademicRecord::addCompletedCourse(int courseId, float score) {
    if (!hasCompletedCourse(courseId) && score >= passingScore) {
        completedCoursesScore[courseId] = score;
    }
}

void AcademicRecord::addFailedCourse(int courseId, int offeringId, float score) {
    if (score < passingScore) {
        failedCourses[courseId] = {offeringId, score};
    }
}
