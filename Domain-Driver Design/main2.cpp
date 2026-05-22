
#include "data/StudentRepository.hpp"
#include "data/CourseRepository.hpp"
#include "data/CourseOfferingRepository.hpp"
#include "data/RegistrationRepository.hpp"
#include "domain/RegistrationService.hpp"
#include "presentation/RegistrationController.hpp"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;


void writeHtmlHeader(ofstream &out) {
    out << "<!DOCTYPE html>\n<html lang='en'>\n<head>\n"
        << "<meta charset='UTF-8'>\n"
        << "<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
        << "<title>University Registration System</title>\n"
        << "<style>\n"
        << ":root { --primary: #4f46e5; --primary-light: #e0e7ff; --success: #10b981; --success-bg: #d1fae5; --danger: #ef4444; --danger-bg: #fee2e2; --bg: #f3f4f6; --surface: #ffffff; --text-main: #1f2937; --text-muted: #6b7280; --border: #e5e7eb; }\n"
        << "body { font-family: system-ui, -apple-system, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif; background-color: var(--bg); color: var(--text-main); line-height: 1.5; margin: 0; padding: 40px 20px; }\n"
        << ".container { max-width: 1100px; margin: 0 auto; }\n"
        << "h1 { text-align: center; color: var(--text-main); font-size: 2.5rem; margin-bottom: 40px; font-weight: 800; }\n"
        << ".card { background: var(--surface); border-radius: 12px; box-shadow: 0 4px 6px -1px rgba(0, 0, 0, 0.1), 0 2px 4px -1px rgba(0, 0, 0, 0.06); padding: 24px; margin-bottom: 24px; border-top: 4px solid var(--primary); }\n"
        << ".card-header { font-size: 1.1rem; font-weight: 700; color: var(--primary); margin-bottom: 16px; text-transform: uppercase; letter-spacing: 0.05em; border-bottom: 2px solid var(--primary-light); padding-bottom: 8px; display: inline-block; }\n"
        << ".alert { padding: 12px 16px; border-radius: 8px; font-weight: 500; display: flex; align-items: center; margin-bottom: 8px; }\n"
        << ".alert.success { background-color: var(--success-bg); color: #065f46; border-left: 4px solid var(--success); }\n"
        << ".alert.error { background-color: var(--danger-bg); color: #991b1b; border-left: 4px solid var(--danger); }\n"
        << "table { width: 100%; border-collapse: collapse; margin-top: 10px; font-size: 0.95rem; }\n"
        << "th, td { padding: 12px 16px; text-align: left; border-bottom: 1px solid var(--border); }\n"
        << "th { background-color: #f9fafb; font-weight: 600; color: var(--text-muted); text-transform: uppercase; font-size: 0.85rem; letter-spacing: 0.05em; }\n"
        << "tr:hover td { background-color: #f9fafb; }\n"
        << ".badge { display: inline-flex; align-items: center; padding: 4px 10px; border-radius: 9999px; font-size: 0.75rem; font-weight: 700; text-transform: uppercase; }\n"
        << ".badge.open { background-color: var(--success-bg); color: var(--success); }\n"
        << ".badge.full { background-color: var(--danger-bg); color: var(--danger); }\n"
        << ".info-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 16px; margin-bottom: 16px; }\n"
        << ".info-item { background: #f9fafb; padding: 12px; border-radius: 8px; border: 1px solid var(--border); }\n"
        << ".info-label { display: block; font-size: 0.8rem; color: var(--text-muted); margin-bottom: 4px; text-transform: uppercase; }\n"
        << ".info-value { font-size: 1.1rem; font-weight: 600; color: var(--text-main); }\n"
        << "</style>\n"
        << "</head>\n<body>\n"
        << "<div class='container'>\n"
        << "<h1>🎓 University Registration System</h1>\n";
}

void writeHtmlFooter(ofstream &out) {
    out << "</div>\n</body>\n</html>\n";
}

void print_offerings_table(ofstream &out, const vector<CourseOfferingView> &offerings) {
    if (offerings.empty()) {
        out << "<div class='alert' style='background: #f3f4f6; color: #4b5563;'><em>No offerings available.</em></div>\n";
        return;
    }
    
    out << "<table>\n"
        << "<thead><tr>"
        << "<th>Offering ID</th><th>Course Code</th><th>Title</th><th>Units</th><th>Class Time</th><th>Exam Time</th><th>Capacity</th><th>Available</th><th>Status</th>"
        << "</tr></thead>\n<tbody>\n";
        
    for (const auto &offer : offerings) {
        string statusBadge = offer.isFull ? "<span class='badge full'>FULL</span>" : "<span class='badge open'>OPEN</span>";
        out << "<tr>"
            << "<td><strong>" << offer.offeringId << "</strong></td>"
            << "<td>" << offer.courseCode << "</td>"
            << "<td>" << offer.courseTitle << "</td>"
            << "<td>" << offer.courseNumOfUnits << "</td>"
            << "<td>" << offer.classTime << "</td>"
            << "<td>" << offer.examTime << "</td>"
            << "<td>" << offer.capacity << "</td>"
            << "<td><strong>" << offer.availableSeats << "</strong></td>"
            << "<td>" << statusBadge << "</td>"
            << "</tr>\n";
    }
    out << "</tbody>\n</table>\n";
}

void viewAvailableOfferings(RegistrationController &controller, int studentId, ofstream &out) {
    auto offerings = controller.viewAvailableOfferings(studentId);
    out << "<div class='card'>\n";
    out << "<div class='card-header'>View Available Offerings (Student ID: " << studentId << ")</div>\n";
    
    if (offerings.status == ResponseStatus::Success) {
        print_offerings_table(out, offerings.body);
    } else {
        out << "<div class='alert error'>✖ " << offerings.message << "</div>\n";
    }
    out << "</div>\n";
}

void registerOffering(RegistrationController &controller, int registrationId, int offeringId, ofstream &out) {
    auto registerResult = controller.registerOffering(registrationId, offeringId);
    out << "<div class='card'>\n";
    out << "<div class='card-header'>Action: Register Course</div>\n";
    
    string cssClass = (registerResult.status == ResponseStatus::Success) ? "success" : "error";
    string icon = (registerResult.status == ResponseStatus::Success) ? "✔ " : "✖ ";
    
    out << "<div class='alert " << cssClass << "'>" << icon 
        << "<strong>[Reg ID: " << registrationId << " | Offering ID: " << offeringId << "]</strong> &nbsp;&mdash;&nbsp; " 
        << registerResult.message << "</div>\n";
    out << "</div>\n";
}

void drop(RegistrationController &controller, int registrationId, int offeringId, ofstream &out) {
    auto result = controller.drop(registrationId, offeringId);
    out << "<div class='card'>\n";
    out << "<div class='card-header'>Action: Drop Course</div>\n";
    
    string cssClass = (result.status == ResponseStatus::Success) ? "success" : "error";
    string icon = (result.status == ResponseStatus::Success) ? "✔ " : "✖ ";
    
    out << "<div class='alert " << cssClass << "'>" << icon 
        << "<strong>[Reg ID: " << registrationId << " | Offering ID: " << offeringId << "]</strong> &nbsp;&mdash;&nbsp; " 
        << result.message << "</div>\n";
    out << "</div>\n";
}

void joinWaitlist(RegistrationController &controller, int registrationId, int offeringId, ofstream &out) {
    auto result = controller.joinWaitlist(registrationId, offeringId);
    out << "<div class='card'>\n";
    out << "<div class='card-header'>Action: Join Waitlist</div>\n";
    
    string cssClass = (result.status == ResponseStatus::Success) ? "success" : "error";
    string icon = (result.status == ResponseStatus::Success) ? "✔ " : "✖ ";
    
    out << "<div class='alert " << cssClass << "'>" << icon 
        << "<strong>[Reg ID: " << registrationId << " | Offering ID: " << offeringId << "]</strong> &nbsp;&mdash;&nbsp; " 
        << result.message << "</div>\n";
    out << "</div>\n";
}

void leaveWaitlist(RegistrationController &controller, int registrationId, int offeringId, ofstream &out) {
    auto result = controller.leaveWaitlist(registrationId, offeringId);
    out << "<div class='card'>\n";
    out << "<div class='card-header'>Action: Leave Waitlist</div>\n";
    
    string cssClass = (result.status == ResponseStatus::Success) ? "success" : "error";
    string icon = (result.status == ResponseStatus::Success) ? "✔ " : "✖ ";
    
    out << "<div class='alert " << cssClass << "'>" << icon 
        << "<strong>[Reg ID: " << registrationId << " | Offering ID: " << offeringId << "]</strong> &nbsp;&mdash;&nbsp; " 
        << result.message << "</div>\n";
    out << "</div>\n";
}

void getStudentRegistration(RegistrationController &controller, int registrationId, ofstream &out) {
    auto registration = controller.getStudentRegistration(registrationId);
    out << "<div class='card'>\n";
    out << "<div class='card-header'>Student Schedule</div>\n";
    
    if (registration.status == ResponseStatus::Success) {
        // اطلاعات کلی دانشجو
        out << "<div class='info-grid'>\n"
            << "<div class='info-item'><span class='info-label'>Student Name</span><span class='info-value'>" << registration.body.studentName << "</span></div>\n"
            << "<div class='info-item'><span class='info-label'>Student ID</span><span class='info-value'>" << registration.body.studentId << "</span></div>\n"
            << "<div class='info-item'><span class='info-label'>Registration ID</span><span class='info-value'>" << registration.body.registrationId << "</span></div>\n"
            << "</div>\n";
        
        if (registration.body.offerings.empty()) {
            out << "<div class='alert' style='background: #f3f4f6; color: #4b5563;'><em>[No courses registered yet]</em></div>\n";
        } else {
            int totalUnits = 0;
            out << "<table>\n"
                << "<thead><tr><th>Course Code</th><th>Title</th><th>Units</th><th>Class Time</th><th>Exam Time</th></tr></thead>\n"
                << "<tbody>\n";
                
            for (const auto &offer : registration.body.offerings) {
                out << "<tr>"
                    << "<td><strong>" << offer.courseCode << "</strong></td>"
                    << "<td>" << offer.courseTitle << "</td>"
                    << "<td>" << offer.courseNumOfUnits << "</td>"
                    << "<td>" << offer.classTime << "</td>"
                    << "<td>" << offer.examTime << "</td>"
                    << "</tr>\n";
                totalUnits += offer.courseNumOfUnits;
            }
            out << "</tbody>\n</table>\n";
            
            out << "<div style='margin-top: 16px; padding: 12px; background: #eef2ff; color: #3730a3; border-radius: 8px; text-align: right;'>\n"
                << "<strong>Total Registered Units: <span style='font-size: 1.2rem;'>" << totalUnits << "</span></strong>\n"
                << "</div>\n";
        }
    } else {
        out << "<div class='alert error'>✖ " << registration.message << "</div>\n";
    }
    out << "</div>\n";
}

void courseInfo(RegistrationController &controller, int offeringId, ofstream &out) {
    auto CourseInfo = controller.courseInfo(offeringId);
    out << "<div class='card'>\n";
    out << "<div class='card-header'>Course Information Details</div>\n";
    
    if (CourseInfo.status == ResponseStatus::Success) {
        out << "<div class='info-grid'>\n"
            << "<div class='info-item'><span class='info-label'>Course ID</span><span class='info-value'>" << CourseInfo.body.courseId << "</span></div>\n"
            << "<div class='info-item'><span class='info-label'>Course Code</span><span class='info-value'>" << CourseInfo.body.courseCode << "</span></div>\n"
            << "<div class='info-item'><span class='info-label'>Title</span><span class='info-value'>" << CourseInfo.body.courseTitle << "</span></div>\n"
            << "<div class='info-item'><span class='info-label'>Units</span><span class='info-value'>" << CourseInfo.body.courseNumOfUnits << "</span></div>\n"
            << "</div>\n";
        
        out << "<div style='margin-bottom: 20px;'><strong>Prerequisites:</strong> ";
        if (!CourseInfo.body.prerequisites.empty()) {
            for (size_t i = 0; i < CourseInfo.body.prerequisites.size() - 1; i++) {
                out << "<span class='badge' style='background: #e5e7eb; color: #374151; margin-right: 4px;'>" << CourseInfo.body.prerequisites[i] << "</span>";
            }
            out << "<span class='badge' style='background: #e5e7eb; color: #374151;'>" << CourseInfo.body.prerequisites.back() << "</span></div>\n";
        } else {
            out << "<span class='badge' style='background: #e5e7eb; color: #374151;'>None</span></div>\n";
        }
        
        out << "<h4 style='color: var(--text-main); margin-top: 24px; margin-bottom: 12px;'>Associated Offerings:</h4>\n";
        print_offerings_table(out, CourseInfo.body.offerings);
    } else {
        out << "<div class='alert error'>✖ " << CourseInfo.message << "</div>\n";
    }
    out << "</div>\n";
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


    ofstream out("output.html");
    writeHtmlHeader(out);


    viewAvailableOfferings(controller, 2, out);
    courseInfo(controller, 1, out);

    registerOffering(controller, 1, 1, out);

    registerOffering(controller, 2, 4, out);

    registerOffering(controller, 2, 1, out);
    registerOffering(controller, 2, 2, out);

    registerOffering(controller, 1, 3, out);
    registerOffering(controller, 1, 4, out);

    joinWaitlist(controller, 3, 1, out);
    registerOffering(controller, 3, 1, out);
    registerOffering(controller, 4, 1, out);
    joinWaitlist(controller, 4, 1, out);
    joinWaitlist(controller, 5, 1, out);
    leaveWaitlist(controller, 5, 1, out);


    getStudentRegistration(controller, 4, out);
    drop(controller, 2, 1, out);
    getStudentRegistration(controller, 4, out);


    getStudentRegistration(controller, 1, out);
    getStudentRegistration(controller, 2, out);
    getStudentRegistration(controller, 3, out);
    getStudentRegistration(controller, 4, out);

    viewAvailableOfferings(controller, 2, out);

    writeHtmlFooter(out);
    out.close();

    cout << "Success! Please open 'output.html' in your web browser." << endl;
    return 0;
}
