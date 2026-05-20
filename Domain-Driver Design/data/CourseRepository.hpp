#pragma once

#include "BaseRepository.hpp"
#include "../domain/Course.hpp"

using namespace std;

class CourseRepository : public BaseRepository<Course> {
public:
    static shared_ptr<CourseRepository> repo();

private:
    static shared_ptr<CourseRepository> repo_;
};
