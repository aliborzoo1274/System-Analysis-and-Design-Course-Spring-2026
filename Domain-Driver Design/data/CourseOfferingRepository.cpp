#include "CourseOfferingRepository.hpp"

shared_ptr<CourseOfferingRepository> CourseOfferingRepository::repo_ = nullptr;

CourseOfferingRepository::CourseOfferingRepository() {

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

    auto offer1 = make_shared<CourseOffering>(1, 101, "A", cs101GroupA, cs101ExamA, 2);
    auto offer2 = make_shared<CourseOffering>(2, 101, "B", cs101GroupB, cs101ExamB, 2);
    auto offer3 = make_shared<CourseOffering>(3, 201, "A", cs201GroupA, cs201ExamA, 2);
    auto offer4 = make_shared<CourseOffering>(4, 102, "A", math101GroupA, math101ExamA, 2);

    save(offer1);
    save(offer2);
    save(offer3);
    save(offer4);
}

shared_ptr<CourseOfferingRepository> CourseOfferingRepository::repo() {

    if (repo_ == nullptr) {
        repo_ = make_shared<CourseOfferingRepository>();
    }

    return repo_;
}
