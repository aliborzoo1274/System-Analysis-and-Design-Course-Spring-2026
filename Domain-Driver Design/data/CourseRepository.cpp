#include "CourseRepository.hpp"

using namespace std;

shared_ptr<CourseRepository> CourseRepository::repo_ = nullptr;

shared_ptr<CourseRepository> CourseRepository::repo() {
    if (!repo_) {
        repo_ = make_shared<CourseRepository>();
    }
    return repo_;
}
