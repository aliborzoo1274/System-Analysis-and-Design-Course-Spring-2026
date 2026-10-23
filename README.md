# Course Registration System

An in-memory C++ course-registration system developed for the **System Analysis and Design** course. The project models students, courses, course offerings, registrations, academic records, schedules, exams, and waitlists through a small layered design.

The repository includes:

- A console demonstration in [`Domain-Driver Design/main.cpp`](./Domain-Driver%20Design/main.cpp)
- An HTML-report demonstration in [`Domain-Driver Design/main2.cpp`](./Domain-Driver%20Design/main2.cpp)
- Domain services that enforce registration rules
- In-memory repositories for students, courses, offerings, and registrations
- Presentation-layer response and view models

> **Note:** This is a demonstration application. Data is seeded in code and is not persisted to a database.

## Features

- Browse available course offerings for a student
- View course details, prerequisites, and all of its offerings
- Register a student for an offering
- Drop a confirmed registration
- Join and leave an offering waitlist
- Automatically promote the first waitlisted registration when a seat becomes available
- Prevent invalid registrations caused by:
  - Missing students, registrations, courses, or offerings
  - Completed courses
  - Unmet prerequisites
  - Registering for multiple groups of the same course
  - Class-time or exam-time conflicts
  - Full offerings
  - Duplicate registrations or waitlist entries
- Display a student schedule and total registered units

## Architecture

The code follows a simple layered structure:

```text
Domain-Driver Design/
├── data/             In-memory repositories
├── domain/           Business entities and registration rules
├── presentation/     Controllers, responses, and view models
├── main.cpp          Console demonstration
├── main2.cpp         HTML-report demonstration
└── Makefile          MSVC/nmake build configuration
```

### Domain layer

The domain layer contains the core business concepts:

- `Student` and `AcademicRecord`
- `Course` and prerequisite course IDs
- `CourseOffering`, including capacity, class time, exam time, confirmed registrations, and waitlist entries
- `Registration`, which connects a student to selected offerings
- `RegistrationService`, which coordinates repositories and enforces registration rules
- Date and time value objects such as `Date`, `Time`, `ClassTime`, and `ExamTime`

### Data layer

The repository classes are lightweight in-memory stores built on the generic [`BaseRepository`](./Domain-Driver%20Design/data/BaseRepository.hpp). Each repository stores objects by integer ID and supports `findById`, `findAll`, `save`, and `remove`.

### Presentation layer

[`RegistrationController`](./Domain-Driver%20Design/presentation/RegistrationController.hpp) exposes application-facing operations and converts domain objects into presentation-friendly views. [`Response`](./Domain-Driver%20Design/presentation/Response.hpp) reports success or failure with a message and optional response body.

## Requirements

- Windows
- Microsoft Visual C++ with `cl.exe`
- `nmake` for the provided Makefile
- C++17 support

The Makefile uses these compiler settings:

```text
/std:c++17 /W4 /O2 /EHsc
```

Open a **Developer Command Prompt for Visual Studio** (or run the equivalent Visual Studio environment setup) so that `cl` and `nmake` are available on `PATH`.

## Build and run the console demo

From the repository root:

```bat
cd "Domain-Driver Design"
nmake
registration.exe
```

The executable runs a scripted scenario and prints:

- Available offerings
- Course information
- Registration, drop, and waitlist results
- Student schedules
- Total registered units

To remove the executable and generated object files:

```bat
nmake clean
```

Generated object files are ignored by Git through [`.gitignore`](./.gitignore).

## Generate the HTML report

The provided Makefile builds [`main.cpp`](./Domain-Driver%20Design/main.cpp), so `main2.cpp` must be compiled separately when the HTML demonstration is needed.

From `Domain-Driver Design`:

```bat
cl /nologo /std:c++17 /W4 /O2 /EHsc ^
  main2.cpp ^
  data\StudentRepository.cpp ^
  data\CourseRepository.cpp ^
  data\CourseOfferingRepository.cpp ^
  data\RegistrationRepository.cpp ^
  domain\AcademicRecord.cpp ^
  domain\ClassTime.cpp ^
  domain\Course.cpp ^
  domain\CourseOffering.cpp ^
  domain\Date.cpp ^
  domain\ExamTime.cpp ^
  domain\Registration.cpp ^
  domain\RegistrationService.cpp ^
  domain\Student.cpp ^
  domain\Time.cpp ^
  presentation\RegistrationController.cpp ^
  /Fe:registration-html.exe
registration-html.exe
start output.html
```

The HTML demo writes `output.html` to the current working directory. The file is generated output and is not part of the source model.

## Seed data

The demos initialize the repositories with the following IDs:

### Students and registrations

| Student ID | Name | Registration ID |
|---:|---|---:|
| 1 | Ali | 1 |
| 2 | Amin | 2 |
| 3 | Reza | 3 |
| 4 | Sara | 4 |
| 5 | Nima | 5 |

Student 1 has completed courses `101` and `103`, with grades recorded in the demo. The other students start with empty academic records.

### Courses

| Course ID | Code | Title | Units | Prerequisites |
|---:|---|---|---:|---|
| 101 | CS101 | Intro to CS | 3 | None |
| 102 | CS102 | Data Structures | 3 | 101 |
| 103 | CS103 | Algorithms | 3 | 102 |
| 201 | CS201 | Operating Systems | 4 | 103 |
| 202 | CS202 | Databases | 3 | 102 |

### Offerings

| Offering ID | Course ID | Capacity |
|---:|---:|---:|
| 1 | 101 | 2 |
| 2 | 101 | 2 |
| 3 | 201 | 2 |
| 4 | 102 | 2 |

Class and examination schedules are defined in `initOfferingRepo` in the selected demo entry point.

## Registration workflow

The typical application flow is:

1. Obtain the singleton in-memory repositories.
2. Seed students, courses, offerings, and registrations.
3. Construct [`RegistrationService`](./Domain-Driver%20Design/domain/RegistrationService.hpp).
4. Construct [`RegistrationController`](./Domain-Driver%20Design/presentation/RegistrationController.hpp).
5. Query offerings or course information.
6. Register, drop, or manage waitlist entries through the controller.
7. Read the returned `Response<T>` status, message, and body.

The domain service is intended to be the single place where registration rules are applied; callers should not mutate registration state directly.

## Extending the project

To add a new domain behavior:

1. Add or update the relevant entity in `domain/`.
2. Add repository access if the behavior needs another aggregate.
3. Implement validation and state changes in `RegistrationService`.
4. Expose the operation through `RegistrationController`.
5. Add or update a view model in `presentation/Views.hpp` when the result needs a presentation-specific shape.
6. Exercise the behavior from `main.cpp` or `main2.cpp`.
7. Add any new `.cpp` file to `SRC` in the Makefile.

## Known scope and limitations

- All state exists only in memory and is reset every time a demo starts.
- There is no database, authentication, web server, or interactive command-line menu.
- The sample data and scenario are hard-coded in the demo entry points.
- The Makefile targets the console demo; the HTML demo is an alternate entry point.
- The repository is intended as a course project and is not packaged as a production enrollment service.
