#pragma once

#include "BaseRepository.hpp"
#include "../domain/Registration.hpp"

#include <memory>

using namespace std;

class RegistrationRepository : public BaseRepository<Registration> {
public:
    RegistrationRepository();

    static shared_ptr<RegistrationRepository> repo();

private:
    static shared_ptr<RegistrationRepository> repo_;
};
