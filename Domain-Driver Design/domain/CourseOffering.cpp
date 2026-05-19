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
    return capacity - static_cast<int>(registeredStudentIds.size());
}

bool CourseOffering::isFull() const {
    return availableSeats() <= 0;
}

bool CourseOffering::isStudentRegistered(int studentId) const {
    return find(registeredStudentIds.begin(), registeredStudentIds.end(), studentId) != registeredStudentIds.end();
}

bool CourseOffering::isStudentWaitlisted(int studentId) const {
    return find(waitlistStudentIds.begin(), waitlistStudentIds.end(), studentId) != waitlistStudentIds.end();
}

bool CourseOffering::registerStudent(int studentId) {
    if (isFull() || isStudentRegistered(studentId)) {
        return false;
    }

    registeredStudentIds.push_back(studentId);
    return true;
}

bool CourseOffering::dropStudent(int studentId) {
    auto it = find(registeredStudentIds.begin(), registeredStudentIds.end(), studentId);
    if (it == registeredStudentIds.end()) {
        return false;
    }

    registeredStudentIds.erase(it);
    return true;
}

bool CourseOffering::joinWaitlist(int studentId) {
    if (isStudentRegistered(studentId) || isStudentWaitlisted(studentId)) {
        return false;
    }

    waitlistStudentIds.push_back(studentId);
    return true;
}

bool CourseOffering::leaveWaitlist(int studentId) {
    auto it = find(waitlistStudentIds.begin(), waitlistStudentIds.end(), studentId);
    if (it == waitlistStudentIds.end()) {
        return false;
    }

    waitlistStudentIds.erase(it);
    return true;
}

const vector<int>& CourseOffering::getRegisteredStudentIds() const {
    return registeredStudentIds;
}

const vector<int>& CourseOffering::getWaitlistStudentIds() const {
    return waitlistStudentIds;
}
