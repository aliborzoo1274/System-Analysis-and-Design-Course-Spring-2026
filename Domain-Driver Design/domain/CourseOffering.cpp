#include "CourseOffering.hpp"

#include <algorithm>

using namespace std;

CourseOffering::CourseOffering() : id(0), courseId(0), capacity(0) {}

CourseOffering::CourseOffering(int id, int courseId, const ClassTime& classTime, const ExamTime& examTime, int capacity)
    : id(id), courseId(courseId), classTime(classTime), examTime(examTime), capacity(capacity) {}

int CourseOffering::getId() const {
    return id;
}

int CourseOffering::getCourseId() const {
    return courseId;
}

const ClassTime& CourseOffering::getClassTime() const {
    return classTime;
}

const ExamTime& CourseOffering::getExamTime() const {
    return examTime;
}

int CourseOffering::getCapacity() const {
    return capacity;
}

int CourseOffering::availableSeats() const {
    return capacity - static_cast<int>(confirmedRegistrationIds.size());
}

bool CourseOffering::isFull() const {
    return availableSeats() <= 0;
}

bool CourseOffering::isRegistrationConfirmed(int registrationId) const {
    return find(confirmedRegistrationIds.begin(), confirmedRegistrationIds.end(), registrationId) != confirmedRegistrationIds.end();
}

bool CourseOffering::isRegistrationWaitlisted(int registrationId) const {
    return find(waitlistRegistrationIds.begin(), waitlistRegistrationIds.end(), registrationId) != waitlistRegistrationIds.end();
}

bool CourseOffering::confirmRegistration(int registrationId) {
    if (isFull() || isRegistrationConfirmed(registrationId)) {
        return false;
    }

    confirmedRegistrationIds.push_back(registrationId);
    return true;
}

bool CourseOffering::dropRegistration(int registrationId) {
    auto it = find(confirmedRegistrationIds.begin(), confirmedRegistrationIds.end(), registrationId);
    if (it == confirmedRegistrationIds.end()) {
        return false;
    }

    confirmedRegistrationIds.erase(it);
    return true;
}

bool CourseOffering::joinWaitlist(int registrationId) {
    if (isRegistrationConfirmed(registrationId) || isRegistrationWaitlisted(registrationId)) {
        return false;
    }

    waitlistRegistrationIds.push_back(registrationId);
    return true;
}

bool CourseOffering::leaveWaitlist(int registrationId) {
    auto it = find(waitlistRegistrationIds.begin(), waitlistRegistrationIds.end(), registrationId);
    if (it == waitlistRegistrationIds.end()) {
        return false;
    }

    waitlistRegistrationIds.erase(it);
    return true;
}

const vector<int>& CourseOffering::getConfirmedRegistrationIds() const {
    return confirmedRegistrationIds;
}

const vector<int>& CourseOffering::getWaitlistRegistrationIds() const {
    return waitlistRegistrationIds;
}
