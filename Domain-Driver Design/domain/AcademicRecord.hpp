#pragma once
#include <vector>
using namespace std;

class AcademicRecord {
public:
    AcademicRecord();
    AcademicRecord(const vector<int>& completedCourseIds);

    const vector<int>& getCompletedCourseIds() const;
    bool hasCompletedCourse(int courseId) const;
    void addCompletedCourse(int courseId);

private:
    float GPA;
    vector<int> completedCourseIds;

};
