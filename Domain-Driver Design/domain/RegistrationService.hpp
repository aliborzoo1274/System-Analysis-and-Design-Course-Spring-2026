#pragma once

#include "Course.hpp"
#include "CourseOffering.hpp"
#include "Registration.hpp"
#include "Student.hpp"
#include "../data/CourseOfferingRepository.hpp"
#include "../data/CourseRepository.hpp"
#include "../data/RegistrationRepository.hpp"
#include "../data/StudentRepository.hpp"

#include <memory>
#include <string>
#include <vector>

using namespace std;

struct ServiceResult {
    bool success;
    string message;

    static ServiceResult ok(const string& message = "");
    static ServiceResult fail(const string& message);
};

class RegistrationService {
public:
    RegistrationService(const shared_ptr<StudentRepository>& studentRepo,
                        const shared_ptr<CourseRepository>& courseRepo,
                        const shared_ptr<CourseOfferingRepository>& offeringRepo,
                        const shared_ptr<RegistrationRepository>& registrationRepo);

    vector<shared_ptr<CourseOffering>> getAvailableOfferings(int studentId);

    ServiceResult confirmRegistration(int registrationId, int offeringId);
    ServiceResult dropRegistration(int registrationId, int offeringId);
    ServiceResult joinWaitlist(int registrationId, int offeringId);
    ServiceResult leaveWaitlist(int registrationId, int offeringId);

    shared_ptr<Registration> getRegistration(int registrationId);
    shared_ptr<CourseOffering> getOffering(int offeringId);
    shared_ptr<Course> getCourse(int courseId);
    shared_ptr<Student> getStudent(int studentId);
    vector<shared_ptr<CourseOffering>> getOfferingsForCourse(int courseId);

private:
    shared_ptr<StudentRepository> studentRepo;
    shared_ptr<CourseRepository> courseRepo;
    shared_ptr<CourseOfferingRepository> offeringRepo;
    shared_ptr<RegistrationRepository> registrationRepo;

    bool hasSameCourseAlready(const Registration& registration, int courseId) const;
    bool hasTimeConflict(const Registration& registration, const CourseOffering& newOffering) const;

};
