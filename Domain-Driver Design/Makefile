APP = registration

CXX = cl
CXXFLAGS = /nologo /std:c++17 /W4 /O2 /EHsc

# Source files – use backslashes for Windows paths
SRC = \
	main.cpp \
	data\StudentRepository.cpp \
	data\CourseRepository.cpp \
	data\CourseOfferingRepository.cpp \
	data\RegistrationRepository.cpp \
	domain\AcademicRecord.cpp \
	domain\ClassTime.cpp \
	domain\Course.cpp \
	domain\CourseOffering.cpp \
	domain\Date.cpp \
	domain\ExamTime.cpp \
	domain\Registration.cpp \
	domain\RegistrationService.cpp \
	domain\Student.cpp \
	domain\Time.cpp \
	presentation\RegistrationController.cpp

# Object files mirror the source folder structure
OBJ = $(SRC:.cpp=.obj)

all: $(APP).exe

# Link – $(OBJ) now includes the subfolder paths
$(APP).exe: $(OBJ)
	$(CXX) $(CXXFLAGS) /Fe$@ $(OBJ)

# Compile rule – /Fo$@ tells cl to output exactly where nmake expects the .obj
.cpp.obj:
	$(CXX) $(CXXFLAGS) /c $< /Fo$@

# Clean – the leading '-' ignores errors (e.g., missing files)
clean:
	-del /Q $(APP).exe $(OBJ)

.PHONY: all clean