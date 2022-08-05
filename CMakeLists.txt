cmake_minimum_required(VERSION 3.14)

# set the project name
project(FinancialData VERSION 1.0)

configure_file(FinancialData.h.in FinancialData.h)

set(CMAKE_CXX_STANDARD 14)

find_package(cpprestsdk REQUIRED)

# add the executable
add_executable(${PROJECT_NAME}) 

target_sources(${PROJECT_NAME} PRIVATE main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})

add_subdirectory(resources)
add_subdirectory(helpers)

target_link_libraries(${PROJECT_NAME} 
    resources
    helpers
    cpprestsdk::cpprest
)