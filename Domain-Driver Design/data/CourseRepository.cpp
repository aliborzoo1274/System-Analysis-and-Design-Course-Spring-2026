#include "CourseRepository.hpp"

using namespace std;

shared_ptr<CourseRepository> CourseRepository::repo_ = nullptr;

shared_ptr<CourseRepository> CourseRepository::repo() {
    if (!repo_) {
        repo_ = make_shared<CourseRepository>();
    }
    return repo_;
}

CourseRepository::CourseRepository() {
    save(make_shared<Course>(101, "CS101", "Intro to CS", vector<int>{}));
    save(make_shared<Course>(102, "CS102", "Data Structures", vector<int>{101}));
    save(make_shared<Course>(103, "CS103", "Algorithms", vector<int>{102}));
    save(make_shared<Course>(201, "CS201", "Operating Systems", vector<int>{103}));
    save(make_shared<Course>(202, "CS202", "Databases", vector<int>{102}));
}
