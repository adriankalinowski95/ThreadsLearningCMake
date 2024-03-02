# CMakeList.txt : CMake project for ThreadsLearning, include source and define
# project specific logic here.
#

set (PROJECT_NAME ThreadsLearning)


# Add source to this project's executable.
add_executable (ThreadsLearning "ThreadsLearning.cpp" "ThreadsLearning.h")

find_package(Boost 1.36.0)
target_link_libraries(ThreadsLearning PRIVATE Boost::system)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ThreadsLearning PROPERTY CXX_STANDARD 20)
endif()



target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS NO)

# TODO: Add tests and install targets if needed.
