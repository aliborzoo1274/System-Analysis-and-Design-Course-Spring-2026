#pragma once

#include <string>
#include <vector>

using namespace std;

struct CourseOfferingView {
    int offeringId;
    int courseId;
    string courseCode;
    string courseTitle;
    int courseNumOfUnits;
    string classTime;
    string examTime;
    int capacity;
    int availableSeats;
    bool isFull;
};

struct ResultView {
    bool success;
    string message;
};

struct StudentRegistrationView {
    int registrationId;
    int studentId;
    string studentName;
    vector<CourseOfferingView> offerings;
};

struct CourseInfoView {
    int courseId;
    string courseCode;
    string courseTitle;
    int courseNumOfUnits;
    vector<string> prerequisites;
    vector<CourseOfferingView> offerings;
};
