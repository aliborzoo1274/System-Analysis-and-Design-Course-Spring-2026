#pragma once

#include "ClassTime.hpp"
#include "ExamTime.hpp"

#include <string>
#include <vector>

using namespace std;

class CourseOffering {
public:
    CourseOffering();
    CourseOffering(int id, int courseId, const ClassTime& classTime, const ExamTime& examTime, int capacity);

    int getId() const;
    int getCourseId() const;
    const ClassTime& getClassTime() const;
    const ExamTime& getExamTime() const;
    int getCapacity() const;

    int availableSeats() const;
    bool isFull() const;

    bool isStudentRegistered(int studentId) const;
    bool isStudentWaitlisted(int studentId) const;

    bool registerStudent(int studentId);
    bool dropStudent(int studentId);
    bool joinWaitlist(int studentId);
    bool leaveWaitlist(int studentId);

    const vector<int>& getRegisteredStudentIds() const;
    const vector<int>& getWaitlistStudentIds() const;

private:
    int id;
    int courseId;
    ClassTime classTime;
    ExamTime examTime;
    int capacity;
    vector<int> registeredStudentIds;
    vector<int> waitlistStudentIds;
    
};
