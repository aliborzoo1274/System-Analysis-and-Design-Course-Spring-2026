#include "data/StudentRepository.hpp"
#include "data/CourseRepository.hpp"
#include "data/CourseOfferingRepository.hpp"
#include "data/RegistrationRepository.hpp"
#include "domain/RegistrationService.hpp"
#include "presentation/RegistrationController.hpp"

#include <iostream>

using namespace std;

void initStudentRepo(shared_ptr<StudentRepository> studentRepo) {
	AcademicRecord record1(unordered_map<int, float>{{101, 18.5}, {103, 19}});
    AcademicRecord record2(unordered_map<int, float>{});

    studentRepo->save(make_shared<Student>(1, "Ali", record1));
    studentRepo->save(make_shared<Student>(2, "Amin", record2));
}

void initCourseRepo(shared_ptr<CourseRepository> courseRepo) {
	courseRepo->save(make_shared<Course>(101, "CS101", "Intro to CS", 3, vector<int>{}));
    courseRepo->save(make_shared<Course>(102, "CS102", "Data Structures", 3, vector<int>{101}));
    courseRepo->save(make_shared<Course>(103, "CS103", "Algorithms", 3, vector<int>{102}));
    courseRepo->save(make_shared<Course>(201, "CS201", "Operating Systems", 4, vector<int>{103}));
    courseRepo->save(make_shared<Course>(202, "CS202", "Databases", 3, vector<int>{102}));
}

void initOfferingRepo(shared_ptr<CourseOfferingRepository> offeringRepo) {
	ClassTime cs101GroupA(
        vector<DayOfWeek>{DayOfWeek::Monday, DayOfWeek::Wednesday},
        Time(9, 0),
        Time(10, 30));
    ExamTime cs101ExamA(Date(2026, 6, 1), Time(9, 0), Time(11, 0));

    ClassTime cs101GroupB(
        vector<DayOfWeek>{DayOfWeek::Tuesday, DayOfWeek::Thursday},
        Time(9, 0),
        Time(10, 30));
    ExamTime cs101ExamB(Date(2026, 6, 2), Time(9, 0), Time(11, 0));

    ClassTime cs201GroupA(
        vector<DayOfWeek>{DayOfWeek::Monday, DayOfWeek::Wednesday},
        Time(10, 30),
        Time(12, 0));
    ExamTime cs201ExamA(Date(2026, 6, 1), Time(10, 0), Time(12, 0));

    ClassTime math101GroupA(
        vector<DayOfWeek>{DayOfWeek::Monday, DayOfWeek::Wednesday},
        Time(9, 30),
        Time(11, 0));
    ExamTime math101ExamA(Date(2026, 6, 3), Time(9, 0), Time(11, 0));

    offeringRepo->save(make_shared<CourseOffering>(1, 101, cs101GroupA, cs101ExamA, 2));
    offeringRepo->save(make_shared<CourseOffering>(2, 101, cs101GroupB, cs101ExamB, 2));
    offeringRepo->save(make_shared<CourseOffering>(3, 201, cs201GroupA, cs201ExamA, 2));
    offeringRepo->save(make_shared<CourseOffering>(4, 102, math101GroupA, math101ExamA, 2));
}

void initRegistrationRepo(shared_ptr<RegistrationRepository> registrationRepo) {
	registrationRepo->save(make_shared<Registration>(1, 1));
	registrationRepo->save(make_shared<Registration>(2, 2));
}

void initData(shared_ptr<StudentRepository> studentRepo, shared_ptr<CourseRepository> courseRepo,
			  shared_ptr<CourseOfferingRepository> offeringRepo, shared_ptr<RegistrationRepository> registrationRepo) {

	initStudentRepo(studentRepo);
	initCourseRepo(courseRepo);
	initOfferingRepo(offeringRepo);
	initRegistrationRepo(registrationRepo);
}

void print_offerings(const vector<CourseOfferingView>& offerings) {
	for (const auto& offer : offerings) {
		cout
			<< offer.offeringId << " | "
			<< offer.courseCode << " | "
			<< offer.courseNumOfUnits << " units" << " | "
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

	initData(studentRepo, courseRepo, offeringRepo, registrationRepo);

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
		cout << CourseInfo.body.courseId << " | " << CourseInfo.body.courseCode << " | ";
		cout << CourseInfo.body.courseTitle << " | " << CourseInfo.body.courseNumOfUnits << " units" << "\n";
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
