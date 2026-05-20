#include "StudentRepository.hpp"

using namespace std;

shared_ptr<StudentRepository> StudentRepository::repo_ = nullptr;

shared_ptr<StudentRepository> StudentRepository::repo() {
    if (!repo_) {
        repo_ = make_shared<StudentRepository>();
    }
    return repo_;
}
