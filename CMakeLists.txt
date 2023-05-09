cmake_minimum_required(VERSION 3.0.0)
project(CMAKE_TEST VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(CMAKE_TEST main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package(libusb CONFIG REQUIRED)
target_include_directories(CMAKE_TEST PRIVATE ${LIBUSB_INCLUDE_DIRS})
target_link_libraries(CMAKE_TEST PRIVATE ${LIBUSB_LIBRARIES})

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(CMAKE_TEST PRIVATE nlohmann_json::nlohmann_json)