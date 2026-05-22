#include "RegistrationService.hpp"

using namespace std;

ServiceResult ServiceResult::ok(const string& message) {
    return ServiceResult{true, message};
}

ServiceResult ServiceResult::fail(const string& message) {
    return ServiceResult{false, message};
}

RegistrationService::RegistrationService(const shared_ptr<StudentRepository>& studentRepo,
                                         const shared_ptr<CourseRepository>& courseRepo,
                                         const shared_ptr<CourseOfferingRepository>& offeringRepo,
                                         const shared_ptr<RegistrationRepository>& registrationRepo)
    : studentRepo(studentRepo), courseRepo(courseRepo), offeringRepo(offeringRepo), registrationRepo(registrationRepo) {}

vector<shared_ptr<CourseOffering>> RegistrationService::getAvailableOfferings(int studentId) {
    auto student = getStudent(studentId);
    if (!student) {
        return {};
    }

    return offeringRepo->findAll();
}

shared_ptr<Student> RegistrationService::getStudent(int studentId) {
    return studentRepo->findById(studentId);
}

shared_ptr<CourseOffering> RegistrationService::getOffering(int offeringId) {
    return offeringRepo->findById(offeringId);
}

shared_ptr<Course> RegistrationService::getCourse(int courseId) {
    return courseRepo->findById(courseId);
}

vector<shared_ptr<CourseOffering>> RegistrationService::getOfferingsForCourse(int courseId) {
    vector<shared_ptr<CourseOffering>> result;

    for (auto& offering : offeringRepo->findAll()) {
        if (offering && offering->getCourseId() == courseId) {
            result.push_back(offering);
        }
    }

    return result;
}

shared_ptr<Registration> RegistrationService::getRegistration(int registrationId) {
    return registrationRepo->findById(registrationId);
}

bool RegistrationService::hasSameCourseAlready(const Registration& registration, int courseId) const {
    for (int offeringId : registration.getOfferingIds()) {
        auto offering = offeringRepo->findById(offeringId);
        if (offering && offering->getCourseId() == courseId) {
            return true;
        }
    }

    return false;
}

bool RegistrationService::hasTimeConflict(const Registration& registration, const CourseOffering& newOffering) const {
    for (int offeringId : registration.getOfferingIds()) {
        auto offering = offeringRepo->findById(offeringId);
        if (!offering) {
            continue;
        }

        if (newOffering.getClassTime().overlaps(offering->getClassTime())) {
            return true;
        }

        if (newOffering.getExamTime().overlaps(offering->getExamTime())) {
            return true;
        }
    }

    return false;
}

ServiceResult RegistrationService::confirmRegistration(int registrationId, int offeringId) {
    auto registration = getRegistration(registrationId);
    if (!registration) {
        return ServiceResult::fail("Registration not found.");
    }

    auto student = getStudent(registration->getStudentId());

    auto offering = getOffering(offeringId);
    if (!offering) {
        return ServiceResult::fail("Offering not found.");
    }

    auto course = getCourse(offering->getCourseId());
    if (!course) {
        return ServiceResult::fail("Course not found.");
    }

    if (student->hasCompletedCourse(course->getId())) {
        return ServiceResult::fail("Course already completed.");
    }

    if (!student->hasCompletedPrerequisites(course->getPrerequisiteCourseIds())) {
        return ServiceResult::fail("Prerequisites are not met.");
    }

    if (registration->hasOffering(offeringId)) {
        return ServiceResult::fail("Student already registered for this offering.");
    }

    if (hasSameCourseAlready(*registration, course->getId())) {
        return ServiceResult::fail("Cannot register multiple groups from the same course.");
    }

    if (hasTimeConflict(*registration, *offering)) {
        return ServiceResult::fail("Class time or exam time overlaps with existing registration.");
    }

    if (offering->isFull()) {
        return ServiceResult::fail("Offering is full. Use waitlist.");
    }

    if (!offering->confirmRegistration(registrationId)) {
        return ServiceResult::fail("Registration failed.");
    }

    registration->addOffering(offeringId);
    offeringRepo->save(offering);
    registrationRepo->save(registration);

    return ServiceResult::ok("Registered successfully.");
}

ServiceResult RegistrationService::dropRegistration(int registrationId, int offeringId) {
    auto registration = getRegistration(registrationId);
    if (!registration) {
        return ServiceResult::fail("Registration not found.");
    }

    auto offering = getOffering(offeringId);
    if (!offering) {
        return ServiceResult::fail("Offering not found.");
    }

    if (!registration->hasOffering(offeringId) || !offering->isRegistrationConfirmed(registrationId)) {
        return ServiceResult::fail("Registration is not confirmed for this offering.");
    }

    offering->dropRegistration(registrationId);
    registration->removeOffering(offeringId);

    if (!offering->getWaitlistRegistrationIds().empty() && offering->availableSeats() > 0) {
        int nextRegistrationId = offering->getWaitlistRegistrationIds().front();
        offering->leaveWaitlist(nextRegistrationId);
        offering->confirmRegistration(nextRegistrationId);

        auto nextRegistration = getRegistration(nextRegistrationId);
        nextRegistration->addOffering(offeringId);
        registrationRepo->save(nextRegistration);
    }

    offeringRepo->save(offering);
    registrationRepo->save(registration);

    return ServiceResult::ok("Dropped successfully.");
}

ServiceResult RegistrationService::joinWaitlist(int registrationId, int offeringId) {
    auto registration = getRegistration(registrationId);
    if (!registration) {
        return ServiceResult::fail("Registration not found.");
    }

    int studentId = registration->getStudentId();

    auto student = getStudent(studentId);
    if (!student) {
        return ServiceResult::fail("Student not found.");
    }

    auto offering = getOffering(offeringId);
    if (!offering) {
        return ServiceResult::fail("Offering not found.");
    }

    auto course = getCourse(offering->getCourseId());
    if (!course) {
        return ServiceResult::fail("Course not found.");
    }

    if (offering->isRegistrationConfirmed(registrationId)) {
        return ServiceResult::fail("Registration is already confirmed for this offering.");
    }

    if (offering->isRegistrationWaitlisted(registrationId)) {
        return ServiceResult::fail("Registration is already on the waitlist.");
    }

    if (student->hasCompletedCourse(course->getId())) {
        return ServiceResult::fail("Course already completed.");
    }

    if (!student->hasCompletedPrerequisites(course->getPrerequisiteCourseIds())) {
        return ServiceResult::fail("Prerequisites are not met.");
    }

    if (hasSameCourseAlready(*registration, course->getId())) {
        return ServiceResult::fail("Cannot register multiple groups from the same course.");
    }

    if (hasTimeConflict(*registration, *offering)) {
        return ServiceResult::fail("Class time or exam time overlaps with existing registration.");
    }

    if (!offering->isFull()) {
        return ServiceResult::fail("Offering has available seats. Register instead.");
    }

    if (!offering->joinWaitlist(registrationId)) {
        return ServiceResult::fail("Waitlist request failed.");
    }

    offeringRepo->save(offering);

    return ServiceResult::ok("Added to waitlist.");
}

ServiceResult RegistrationService::leaveWaitlist(int registrationId, int offeringId) {
    auto offering = getOffering(offeringId);
    if (!offering) {
        return ServiceResult::fail("Offering not found.");
    }

    if (!offering->isRegistrationWaitlisted(registrationId)) {
        return ServiceResult::fail("Registration is not on the waitlist.");
    }

    offering->leaveWaitlist(registrationId);
    offeringRepo->save(offering);

    return ServiceResult::ok("Left waitlist.");
}
