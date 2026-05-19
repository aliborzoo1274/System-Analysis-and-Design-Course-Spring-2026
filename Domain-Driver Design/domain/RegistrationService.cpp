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

shared_ptr<Registration> RegistrationService::getStudentRegistration(int studentId) {
    return registrationRepo->findById(studentId);
}

shared_ptr<Registration> RegistrationService::getOrCreateRegistration(int studentId) {
    auto registration = registrationRepo->findById(studentId);

    if (!registration) {
        registration = make_shared<Registration>(studentId, studentId);
        registrationRepo->save(registration);
    }

    return registration;
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

ServiceResult RegistrationService::registerStudent(int studentId, int offeringId) {
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

    if (student->getRecord().hasCompletedCourse(course->getId())) {
        return ServiceResult::fail("Course already completed.");
    }

    if (!course->prerequisitesMet(student->getRecord())) {
        return ServiceResult::fail("Prerequisites are not met.");
    }

    auto registration = getOrCreateRegistration(studentId);

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

    if (!offering->registerStudent(studentId)) {
        return ServiceResult::fail("Registration failed.");
    }

    registration->addOffering(offeringId);
    offeringRepo->save(offering);
    registrationRepo->save(registration);

    return ServiceResult::ok("Registered successfully.");
}

ServiceResult RegistrationService::dropStudent(int studentId, int offeringId) {
    auto registration = registrationRepo->findById(studentId);
    if (!registration) {
        return ServiceResult::fail("Registration not found.");
    }

    auto offering = getOffering(offeringId);
    if (!offering) {
        return ServiceResult::fail("Offering not found.");
    }

    if (!registration->hasOffering(offeringId) || !offering->isStudentRegistered(studentId)) {
        return ServiceResult::fail("Student is not registered for this offering.");
    }

    offering->dropStudent(studentId);
    registration->removeOffering(offeringId);

    if (!offering->getWaitlistStudentIds().empty() && offering->availableSeats() > 0) {
        int nextStudentId = offering->getWaitlistStudentIds().front();
        offering->leaveWaitlist(nextStudentId);
        offering->registerStudent(nextStudentId);

        auto nextRegistration = getOrCreateRegistration(nextStudentId);
        nextRegistration->addOffering(offeringId);
        registrationRepo->save(nextRegistration);
    }

    offeringRepo->save(offering);
    registrationRepo->save(registration);

    return ServiceResult::ok("Dropped successfully.");
}

ServiceResult RegistrationService::joinWaitlist(int studentId, int offeringId) {
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

    if (offering->isStudentRegistered(studentId)) {
        return ServiceResult::fail("Student is already registered for this offering.");
    }

    if (offering->isStudentWaitlisted(studentId)) {
        return ServiceResult::fail("Student is already on the waitlist.");
    }

    if (student->getRecord().hasCompletedCourse(course->getId())) {
        return ServiceResult::fail("Course already completed.");
    }

    if (!course->prerequisitesMet(student->getRecord())) {
        return ServiceResult::fail("Prerequisites are not met.");
    }

    auto registration = getOrCreateRegistration(studentId);

    if (hasSameCourseAlready(*registration, course->getId())) {
        return ServiceResult::fail("Cannot register multiple groups from the same course.");
    }

    if (hasTimeConflict(*registration, *offering)) {
        return ServiceResult::fail("Class time or exam time overlaps with existing registration.");
    }

    if (!offering->isFull()) {
        return ServiceResult::fail("Offering has available seats. Register instead.");
    }

    if (!offering->joinWaitlist(studentId)) {
        return ServiceResult::fail("Waitlist request failed.");
    }

    offeringRepo->save(offering);

    return ServiceResult::ok("Added to waitlist.");
}

ServiceResult RegistrationService::leaveWaitlist(int studentId, int offeringId) {
    auto offering = getOffering(offeringId);
    if (!offering) {
        return ServiceResult::fail("Offering not found.");
    }

    if (!offering->isStudentWaitlisted(studentId)) {
        return ServiceResult::fail("Student is not on the waitlist.");
    }

    offering->leaveWaitlist(studentId);
    offeringRepo->save(offering);

    return ServiceResult::ok("Left waitlist.");
}
