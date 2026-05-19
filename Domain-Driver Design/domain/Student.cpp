#include "Student.hpp"

using namespace std;

Student::Student() : id(0) {}

Student::Student(int id, const string& name) : id(id), name(name) {}

Student::Student(int id, const string& name, const AcademicRecord& record)
    : id(id), name(name), record(record) {}

int Student::getId() const {
    return id;
}

string Student::getName() const {
    return name;
}

AcademicRecord& Student::getRecord() {
    return record;
}

const AcademicRecord& Student::getRecord() const {
    return record;
}
