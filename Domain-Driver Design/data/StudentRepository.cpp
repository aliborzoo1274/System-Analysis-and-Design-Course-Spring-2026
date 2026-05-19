#include "StudentRepository.hpp"

using namespace std;

shared_ptr<StudentRepository> StudentRepository::repo_ = nullptr;

shared_ptr<StudentRepository> StudentRepository::repo() {
    if (!repo_) {
        repo_ = make_shared<StudentRepository>();
    }
    return repo_;
}

StudentRepository::StudentRepository() {
    AcademicRecord record1(vector<int>{101});
    AcademicRecord record2(vector<int>{});

    save(make_shared<Student>(1, "Ali", record1));
    save(make_shared<Student>(2, "Amin", record2));
}
