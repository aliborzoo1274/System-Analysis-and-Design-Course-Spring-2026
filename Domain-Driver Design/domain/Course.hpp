#pragma once

#include <string>
#include <vector>

using namespace std;

class Course {
public:
    Course();
    Course(int id, const string& code, const string& title, int numOfUnits, const vector<int>& prerequisiteCourseIds);

    int getId() const;
    string getCode() const;
    string getTitle() const;
    int getNumOfUnits() const;
    const vector<int>& getPrerequisiteCourseIds() const;

private:
    int id;
    string code;
    string title;
    int numOfUnits;
    vector<int> prerequisiteCourseIds;

};
