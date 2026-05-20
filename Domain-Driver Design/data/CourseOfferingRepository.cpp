#include "CourseOfferingRepository.hpp"

shared_ptr<CourseOfferingRepository> CourseOfferingRepository::repo_ = nullptr;

shared_ptr<CourseOfferingRepository> CourseOfferingRepository::repo() {

    if (repo_ == nullptr) {
        repo_ = make_shared<CourseOfferingRepository>();
    }

    return repo_;
}
