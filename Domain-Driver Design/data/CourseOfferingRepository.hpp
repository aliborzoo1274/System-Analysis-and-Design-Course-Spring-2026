#pragma once

#include "BaseRepository.hpp"
#include "../domain/CourseOffering.hpp"

#include <memory>

using namespace std;

class CourseOfferingRepository : public BaseRepository<CourseOffering> {
public:
    static shared_ptr<CourseOfferingRepository> repo();

private:
    static shared_ptr<CourseOfferingRepository> repo_;
};
