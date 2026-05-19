#pragma once

#include "AcademicRecord.hpp"

#include <string>

using namespace std;

class Student {
public:
    Student();
    Student(int id, const string& name);
    Student(int id, const string& name, const AcademicRecord& record);

    int getId() const;
    string getName() const;

    AcademicRecord& getRecord();
    const AcademicRecord& getRecord() const;

private:
    int id;
    string name;
    AcademicRecord record;

};
