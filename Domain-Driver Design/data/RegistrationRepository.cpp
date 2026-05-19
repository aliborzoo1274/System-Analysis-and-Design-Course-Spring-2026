#include "RegistrationRepository.hpp"

shared_ptr<RegistrationRepository> RegistrationRepository::repo_ = nullptr;

RegistrationRepository::RegistrationRepository() {}

shared_ptr<RegistrationRepository> RegistrationRepository::repo() {

    if (repo_ == nullptr) {
        repo_ = make_shared<RegistrationRepository>();
    }

    return repo_;
}
