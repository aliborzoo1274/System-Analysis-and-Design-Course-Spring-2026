#include "data/StudentRepository.hpp"
#include "data/CourseRepository.hpp"
#include "data/CourseOfferingRepository.hpp"
#include "data/RegistrationRepository.hpp"
#include "domain/RegistrationService.hpp"
#include "presentation/RegistrationController.hpp"

#include <iostream>

using namespace std;

void print_offerings(const vector<CourseOfferingView>& offerings) {
	for (const auto& offer : offerings) {
		cout
			<< offer.offeringId << " | "
			<< offer.courseCode << " | "
			<< offer.classTime << " | "
			<< offer.examTime << " | "
			<< "Seats: " << offer.availableSeats
			<< "\n";
	}
}

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
		print_offerings(offerings.body);
	}
	else {
		cout << offerings.message << "\n";
	}
	cout << endl;

	auto registerResult = controller.registerOffering(1, 4);
	cout << "Register result: " << registerResult.message << "\n";
	cout << endl;

	auto registration = controller.getStudentRegistration(1);
	if (registration.status == ResponseStatus::Success) {
		cout << "Student schedule:" << "\n";
		for (const auto& offer : registration.body.offerings) {
			cout
				<< offer.courseCode << " | "
				<< offer.classTime
				<< "\n";
		}
	}
	cout << endl;

	auto CourseInfo = controller.courseInfo(4);
	if (CourseInfo.status == ResponseStatus::Success) {
		cout << "Course Info: ";
		cout << CourseInfo.body.courseId << " | " << CourseInfo.body.courseCode << " | " << CourseInfo.body.courseTitle << "\n";
		cout << "Prerequisites: ";
		for (int i = 0; i < CourseInfo.body.prerequisites.size() - 1; i++) {
			cout << CourseInfo.body.prerequisites[i] << " | ";
		}
		cout << CourseInfo.body.prerequisites.back() << "\n";
		cout << "Offerings:" << "\n";
		print_offerings(CourseInfo.body.offerings);
	}

	return 0;
}
