#pragma once

#include "Response.hpp"
#include "Views.hpp"
#include "../domain/RegistrationService.hpp"

#include <memory>
#include <vector>

using namespace std;

class RegistrationController {
public:
    RegistrationController(const shared_ptr<RegistrationService>& service);

    Response<vector<CourseOfferingView>> viewAvailableOfferings(int studentId);
    Response<ResultView> registerOffering(int studentId, int offeringId);
    Response<ResultView> drop(int studentId, int offeringId);
    Response<ResultView> joinWaitlist(int studentId, int offeringId);
    Response<ResultView> leaveWaitlist(int studentId, int offeringId);
    Response<StudentRegistrationView> getStudentRegistration(int studentId);
    Response<CourseInfoView> courseInfo(int offeringId);

private:
    shared_ptr<RegistrationService> service;

    CourseOfferingView toOfferingView(const shared_ptr<CourseOffering>& offering);

};
