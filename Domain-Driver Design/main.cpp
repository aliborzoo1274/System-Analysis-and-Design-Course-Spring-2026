#include "data/StudentRepository.hpp"
#include "data/CourseRepository.hpp"
#include "data/CourseOfferingRepository.hpp"
#include "data/RegistrationRepository.hpp"
#include "domain/RegistrationService.hpp"
#include "presentation/RegistrationController.hpp"

#include <iostream>

using namespace std;

void print_offerings(const vector<CourseOfferingView> &offerings)
{
	if (offerings.empty())
	{
		cout << "  No offerings available.\n";
		return;
	}
	for (const auto &offer : offerings)
	{
		cout
			<< "  - Offering ID: " << offer.offeringId
			<< " | Course: " << offer.courseCode << " (" << offer.courseTitle << ")"
			<< "\n    Units: " << offer.courseNumOfUnits
			<< " | Class: " << offer.classTime
			<< " | Exam: " << offer.examTime
			<< "\n    Capacity: " << offer.capacity
			<< " | Seats Available: " << offer.availableSeats
			<< " | Status: " << (offer.isFull ? "FULL" : "OPEN")
			<< "\n\n";
	}
}

void viewAvailableOfferings(RegistrationController &controller, int studentId)
{
	auto offerings = controller.viewAvailableOfferings(studentId);
	if (offerings.status == ResponseStatus::Success)
	{
		cout << "Available offerings for Student ID " << studentId << ":" << "\n";
		print_offerings(offerings.body);
	}
	else
	{
		cout << offerings.message << "\n";
	}
	cout << endl;
	cout << "---------------------------------------------------------------------------------------------\n";
}

void registerOffering(RegistrationController &controller, int registrationId, int offeringId)
{
	auto registerResult = controller.registerOffering(registrationId, offeringId);
	cout << "Register result (Reg ID: " << registrationId << ", Offering ID: " << offeringId << "): " << registerResult.message << "\n";
	cout << endl;
	cout << "---------------------------------------------------------------------------------------------\n";
}

void drop(RegistrationController &controller, int registrationId, int offeringId)
{
	auto result = controller.drop(registrationId, offeringId);
	cout << "Drop result (Reg ID: " << registrationId << ", Offering ID: " << offeringId << "): " << result.message << "\n";
	cout << endl;
	cout << "---------------------------------------------------------------------------------------------\n";
}

void joinWaitlist(RegistrationController &controller, int registrationId, int offeringId)
{
	auto result = controller.joinWaitlist(registrationId, offeringId);
	cout << "Join waitlist result (Reg ID: " << registrationId << ", Offering ID: " << offeringId << "): " << result.message << "\n";
	cout << endl;
	cout << "---------------------------------------------------------------------------------------------\n";
}

void leaveWaitlist(RegistrationController &controller, int registrationId, int offeringId)
{
	auto result = controller.leaveWaitlist(registrationId, offeringId);
	cout << "Leave waitlist result (Reg ID: " << registrationId << ", Offering ID: " << offeringId << "): " << result.message << "\n";
	cout << endl;
	cout << "---------------------------------------------------------------------------------------------\n";
}

void getStudentRegistration(RegistrationController &controller, int registrationId)
{
	auto registration = controller.getStudentRegistration(registrationId);
	if (registration.status == ResponseStatus::Success)
	{
		cout << "Student Schedule -> " << registration.body.studentName
			 << " (Student ID: " << registration.body.studentId << " | Reg ID: " << registration.body.registrationId << ")\n";

		if (registration.body.offerings.empty())
		{
			cout << "  [No courses registered yet]\n";
		}
		else
		{
			int totalUnits = 0;
			for (const auto &offer : registration.body.offerings)
			{
				cout
					<< "  - " << offer.courseCode << " (" << offer.courseTitle << ")"
					<< " | " << offer.courseNumOfUnits << " units"
					<< "\n    Class: " << offer.classTime
					<< " | Exam: " << offer.examTime
					<< "\n";
				totalUnits += offer.courseNumOfUnits;
			}
			cout << "  --------------------------------\n";
			cout << "  Total Registered Units: " << totalUnits << "\n";
		}
	}
	else
	{
		cout << registration.message << "\n";
	}
	cout << endl;
	cout << "---------------------------------------------------------------------------------------------\n";
}

void courseInfo(RegistrationController &controller, int offeringId)
{
	auto CourseInfo = controller.courseInfo(offeringId);
	if (CourseInfo.status == ResponseStatus::Success)
	{
		cout << "Course Info Details:\n";
		cout << "  Course ID: " << CourseInfo.body.courseId
			 << " | Code: " << CourseInfo.body.courseCode
			 << " | Title: " << CourseInfo.body.courseTitle
			 << " | Units: " << CourseInfo.body.courseNumOfUnits << "\n";

		cout << "  Prerequisites: ";
		if (!CourseInfo.body.prerequisites.empty())
		{
			for (int i = 0; i < CourseInfo.body.prerequisites.size() - 1; i++)
			{
				cout << CourseInfo.body.prerequisites[i] << ", ";
			}
			cout << CourseInfo.body.prerequisites.back() << "\n";
		}
		else
		{
			cout << "None" << "\n";
		}
		cout << "\nAll Offerings for this Course:" << "\n";
		print_offerings(CourseInfo.body.offerings);
	}
	else
	{
		cout << CourseInfo.message << "\n";
	}
	cout << endl;
	cout << "---------------------------------------------------------------------------------------------\n";
}

void initStudentRepo(shared_ptr<StudentRepository> studentRepo)
{
	AcademicRecord record1(unordered_map<int, float>{{101, 18.5}, {103, 19}});
	AcademicRecord record2(unordered_map<int, float>{});
	studentRepo->save(make_shared<Student>(1, "Ali", record1));
	studentRepo->save(make_shared<Student>(2, "Amin", record2));
	studentRepo->save(make_shared<Student>(3, "Reza", AcademicRecord(unordered_map<int, float>{})));
	studentRepo->save(make_shared<Student>(4, "Sara", AcademicRecord(unordered_map<int, float>{})));
	studentRepo->save(make_shared<Student>(5, "Nima", AcademicRecord(unordered_map<int, float>{})));
}

void initCourseRepo(shared_ptr<CourseRepository> courseRepo)
{
	courseRepo->save(make_shared<Course>(101, "CS101", "Intro to CS", 3, vector<int>{}));
	courseRepo->save(make_shared<Course>(102, "CS102", "Data Structures", 3, vector<int>{101}));
	courseRepo->save(make_shared<Course>(103, "CS103", "Algorithms", 3, vector<int>{102}));
	courseRepo->save(make_shared<Course>(201, "CS201", "Operating Systems", 4, vector<int>{103}));
	courseRepo->save(make_shared<Course>(202, "CS202", "Databases", 3, vector<int>{102}));
}

void initOfferingRepo(shared_ptr<CourseOfferingRepository> offeringRepo)
{
	ClassTime cs101GroupA(vector<DayOfWeek>{DayOfWeek::Monday, DayOfWeek::Wednesday}, Time(9, 0), Time(10, 30));
	ExamTime cs101ExamA(Date(2026, 6, 1), Time(9, 0), Time(11, 0));
	ClassTime cs101GroupB(vector<DayOfWeek>{DayOfWeek::Tuesday, DayOfWeek::Thursday}, Time(9, 0), Time(10, 30));
	ExamTime cs101ExamB(Date(2026, 6, 2), Time(9, 0), Time(11, 0));
	ClassTime cs201GroupA(vector<DayOfWeek>{DayOfWeek::Monday, DayOfWeek::Wednesday}, Time(10, 30), Time(12, 0));
	ExamTime cs201ExamA(Date(2026, 6, 1), Time(10, 0), Time(12, 0));
	ClassTime math101GroupA(vector<DayOfWeek>{DayOfWeek::Monday, DayOfWeek::Wednesday}, Time(9, 30), Time(11, 0));
	ExamTime math101ExamA(Date(2026, 6, 3), Time(9, 0), Time(11, 0));

	offeringRepo->save(make_shared<CourseOffering>(1, 101, cs101GroupA, cs101ExamA, 2));
	offeringRepo->save(make_shared<CourseOffering>(2, 101, cs101GroupB, cs101ExamB, 2));
	offeringRepo->save(make_shared<CourseOffering>(3, 201, cs201GroupA, cs201ExamA, 2));
	offeringRepo->save(make_shared<CourseOffering>(4, 102, math101GroupA, math101ExamA, 2));
}

void initRegistrationRepo(shared_ptr<RegistrationRepository> registrationRepo)
{
	registrationRepo->save(make_shared<Registration>(1, 1));
	registrationRepo->save(make_shared<Registration>(2, 2));
	registrationRepo->save(make_shared<Registration>(3, 3));
	registrationRepo->save(make_shared<Registration>(4, 4));
	registrationRepo->save(make_shared<Registration>(5, 5));
}

void initData(shared_ptr<StudentRepository> studentRepo, shared_ptr<CourseRepository> courseRepo,
			  shared_ptr<CourseOfferingRepository> offeringRepo, shared_ptr<RegistrationRepository> registrationRepo)
{
	initStudentRepo(studentRepo);
	initCourseRepo(courseRepo);
	initOfferingRepo(offeringRepo);
	initRegistrationRepo(registrationRepo);
}

int main()
{
	auto studentRepo = StudentRepository::repo();
	auto courseRepo = CourseRepository::repo();
	auto offeringRepo = CourseOfferingRepository::repo();
	auto registrationRepo = RegistrationRepository::repo();

	initData(studentRepo, courseRepo, offeringRepo, registrationRepo);

	auto service = make_shared<RegistrationService>(studentRepo, courseRepo, offeringRepo, registrationRepo);
	RegistrationController controller(service);

	viewAvailableOfferings(controller, 2);
	courseInfo(controller, 1);


	registerOffering(controller, 1, 1);


	registerOffering(controller, 2, 4);


	registerOffering(controller, 2, 1);
	registerOffering(controller, 2, 2);

	registerOffering(controller, 1, 3);
	registerOffering(controller, 1, 4);

	joinWaitlist(controller, 3, 1);

	registerOffering(controller, 3, 1);

	registerOffering(controller, 4, 1);

	joinWaitlist(controller, 4, 1);
	joinWaitlist(controller, 5, 1);

	leaveWaitlist(controller, 5, 1);

	getStudentRegistration(controller, 4);

	drop(controller, 2, 1);

	getStudentRegistration(controller, 4);

	getStudentRegistration(controller, 1);
	getStudentRegistration(controller, 2);
	getStudentRegistration(controller, 3);
	getStudentRegistration(controller, 4);

	viewAvailableOfferings(controller, 2);

	return 0;
}





