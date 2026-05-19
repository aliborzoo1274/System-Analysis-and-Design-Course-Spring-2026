#pragma once

#include "BaseRepository.hpp"
#include "../domain/Student.hpp"

using namespace std;

class StudentRepository : public BaseRepository<Student> {
public:
    static shared_ptr<StudentRepository> repo();
    StudentRepository();

private:

    static shared_ptr<StudentRepository> repo_;
};
