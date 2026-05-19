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
    save(make_shared<Student>(1, "Ali", vector<int>{101}));
    save(make_shared<Student>(2, "Amin", vector<int>{}));
}
