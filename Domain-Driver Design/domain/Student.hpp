#pragma once

#include <string>
#include <vector>

using namespace std;

class Student {
public:
    Student();
    Student(int id, const string& name);
    Student(int id, const string& name, const vector<int>& completedCourseIds);

    int getId() const;
    string getName() const;

    bool hasCompletedCourse(int courseId) const;
    bool hasCompletedPrerequisites(const vector<int>& prerequisites);
    void addCompletedCourse(int courseId);

private:
    int id;
    string name;
    vector<int> completedCourseIds;

};
