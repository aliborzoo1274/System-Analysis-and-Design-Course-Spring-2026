#include "data/StudentRepository.hpp"
#include "data/CourseRepository.hpp"
#include "data/CourseOfferingRepository.hpp"
#include "data/RegistrationRepository.hpp"
#include "domain/RegistrationService.hpp"
#include "presentation/RegistrationController.hpp"

#include <iostream>

using namespace std;

int main() {

	auto studentRepo = StudentRepository::repo();
	auto courseRepo = CourseRepository::repo();
	auto offeringRepo = CourseOfferingRepository::repo();
	auto registrationRepo = RegistrationRepository::repo();

	auto service = make_shared<RegistrationService>(
		studentRepo,
		courseRepo,
		offeringRepo,
		registrationRepo);

	RegistrationController controller(service);

	auto offerings = controller.viewAvailableOfferings(1);
	if (offerings.status == ResponseStatus::Success) {
		cout << "Available offerings:" << "\n";
		for (const auto& offer : offerings.body) {
			cout
				<< offer.offeringId << " | "
				<< offer.courseCode << " | "
				<< "Group " << offer.group << " | "
				<< offer.classTime << " | "
				<< offer.examTime << " | "
				<< "Seats: " << offer.availableSeats
				<< "\n";
		}
	}
	else {
		cout << offerings.message << "\n";
	}

	auto registerResult = controller.registerOffering(1, 4);
	cout << "Register result: " << registerResult.message << "\n";

	auto registration = controller.getStudentRegistration(1);
	if (registration.status == ResponseStatus::Success) {
		cout << "Student schedule:" << "\n";
		for (const auto& offer : registration.body.offerings) {
			cout
				<< offer.courseCode << " "
				<< offer.group << " | "
				<< offer.classTime
				<< "\n";
		}
	}

	return 0;
}
