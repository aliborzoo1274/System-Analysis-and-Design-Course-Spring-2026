#pragma once

#include <string>
#include <vector>

using namespace std;

class Course {
public:
    Course();
    Course(int id, const string& code, const string& title, const vector<int>& prerequisiteCourseIds);

    int getId() const;
    string getCode() const;
    string getTitle() const;
    const vector<int>& getPrerequisiteCourseIds() const;

private:
    int id;
    string code;
    string title;
    vector<int> prerequisiteCourseIds;

};
