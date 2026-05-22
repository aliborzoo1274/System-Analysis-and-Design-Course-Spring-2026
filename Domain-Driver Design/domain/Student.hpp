#pragma once

#include "AcademicRecord.hpp"

#include <string>
#include <vector>

using namespace std;

class Student {
public:
    Student();
    Student(int id, const string& name);
    Student(int id, const string& name, const AcademicRecord& record);

    int getId() const;
    string getName() const;

    bool hasCompletedCourse(int courseId) const;
    bool hasCompletedPrerequisites(const vector<int>& prerequisites);

private:
    int id;
    string name;
    AcademicRecord record;

};
