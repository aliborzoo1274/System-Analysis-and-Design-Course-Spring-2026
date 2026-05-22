#pragma once
#include <vector>
#include <unordered_map>
using namespace std;

class AcademicRecord {
public:
    AcademicRecord();
    AcademicRecord(const unordered_map<int, float>& completedCourseIds);

    bool hasCompletedCourse(int courseId) const;
    bool hasCompletedPrerequisites(const vector<int>& prerequisites);
    void addCompletedCourse(int courseId, float score);
    void addFailedCourse(int courseId, int offeringId, float score);

private:
    unordered_map<int, float> completedCoursesScore;
    unordered_map<int, float> failedCoursesScore;
    unordered_map<int, pair<int, float>> failedCourses;

};
