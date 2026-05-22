#include "RegistrationController.hpp"

using namespace std;

RegistrationController::RegistrationController(const shared_ptr<RegistrationService>& service)
    : service(service) {}

CourseOfferingView RegistrationController::toOfferingView(const shared_ptr<CourseOffering>& offering) {
    CourseOfferingView view{};
    if (!offering) {
        return view;
    }

    auto course = service->getCourse(offering->getCourseId());

    view.offeringId = offering->getId();
    view.courseId = offering->getCourseId();
    view.courseCode = course ? course->getCode() : "";
    view.courseTitle = course ? course->getTitle() : "";
    view.courseNumOfUnits = course->getNumOfUnits();
    view.classTime = offering->getClassTime().toString();
    view.examTime = offering->getExamTime().toString();
    view.capacity = offering->getCapacity();
    view.availableSeats = offering->availableSeats();
    view.isFull = offering->isFull();

    return view;
}

Response<vector<CourseOfferingView>> RegistrationController::viewAvailableOfferings(int studentId) {
    auto student = service->getStudent(studentId);
    if (!student) {
        return Response<vector<CourseOfferingView>>::failure("Student not found.");
    }

    auto offerings = service->getAvailableOfferings(studentId);

    vector<CourseOfferingView> result;
    for (const auto& offering : offerings) {
        result.push_back(toOfferingView(offering));
    }

    return Response<vector<CourseOfferingView>>::success(result, "Available offerings loaded.");
}

Response<ResultView> RegistrationController::registerOffering(int registrationId, int offeringId) {
    auto result = service->confirmRegistration(registrationId, offeringId);
    ResultView view{result.success, result.message};

    if (result.success) {
        return Response<ResultView>::success(view, result.message);
    }

    return Response<ResultView>::failure(view, result.message);
}

Response<ResultView> RegistrationController::drop(int studentId, int offeringId) {
    auto result = service->dropRegistration(studentId, offeringId);
    ResultView view{result.success, result.message};

    if (result.success) {
        return Response<ResultView>::success(view, result.message);
    }

    return Response<ResultView>::failure(view, result.message);
}

Response<ResultView> RegistrationController::joinWaitlist(int studentId, int offeringId) {
    auto result = service->joinWaitlist(studentId, offeringId);
    ResultView view{result.success, result.message};

    if (result.success) {
        return Response<ResultView>::success(view, result.message);
    }

    return Response<ResultView>::failure(view, result.message);
}

Response<ResultView> RegistrationController::leaveWaitlist(int studentId, int offeringId) {
    auto result = service->leaveWaitlist(studentId, offeringId);
    ResultView view{result.success, result.message};

    if (result.success) {
        return Response<ResultView>::success(view, result.message);
    }

    return Response<ResultView>::failure(view, result.message);
}

Response<StudentRegistrationView> RegistrationController::getStudentRegistration(int registrationId) {
    auto registration = service->getRegistration(registrationId);
    if (!registration) {
        return Response<StudentRegistrationView>::failure("Registration not found.");
    }

    auto student = service->getStudent(registration->getStudentId());

    StudentRegistrationView view{};
    view.registrationId = registrationId;
    view.studentId = registration->getStudentId();
    view.studentName = student->getName();

    for (int offeringId : registration->getOfferingIds()) {
        view.offerings.push_back(toOfferingView(service->getOffering(offeringId)));
    }

    return Response<StudentRegistrationView>::success(view, "Student registration loaded.");
}

Response<CourseInfoView> RegistrationController::courseInfo(int offeringId) {
    auto offering = service->getOffering(offeringId);
    if (!offering) {
        return Response<CourseInfoView>::failure("Offering not found.");
    }

    auto course = service->getCourse(offering->getCourseId());
    if (!course) {
        return Response<CourseInfoView>::failure("Course not found.");
    }

    CourseInfoView view{};
    view.courseId = course->getId();
    view.courseCode = course->getCode();
    view.courseTitle = course->getTitle();
    view.courseNumOfUnits = course->getNumOfUnits();

    for (int prerequisiteId : course->getPrerequisiteCourseIds()) {
        auto prerequisite = service->getCourse(prerequisiteId);
        view.prerequisites.push_back(prerequisite ? prerequisite->getTitle() : to_string(prerequisiteId));
    }

    for (const auto& courseOffering : service->getOfferingsForCourse(course->getId())) {
        view.offerings.push_back(toOfferingView(courseOffering));
    }

    return Response<CourseInfoView>::success(view, "Course info loaded.");
}
