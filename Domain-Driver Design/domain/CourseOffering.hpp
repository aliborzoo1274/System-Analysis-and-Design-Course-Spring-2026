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

    bool isRegistrationConfirmed(int registrationId) const;
    bool isRegistrationWaitlisted(int registrationId) const;

    bool confirmRegistration(int registrationId);
    bool dropRegistration(int registrationId);
    bool joinWaitlist(int registrationId);
    bool leaveWaitlist(int registrationId);

    const vector<int>& getConfirmedRegistrationIds() const;
    const vector<int>& getWaitlistRegistrationIds() const;

private:
    int id;
    int courseId;
    ClassTime classTime;
    ExamTime examTime;
    int capacity;
    vector<int> confirmedRegistrationIds;
    vector<int> waitlistRegistrationIds;
    
};
