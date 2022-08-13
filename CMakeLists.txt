cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 14)

# set the project name
project("FinancialData"
        DESCRIPTION "CPP API wrapper for multiple financial data APIs"
        VERSION 1.0)

# Check if we are in the top level project or not
if (NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
message(STATUS "This project has a top-level one called [${CMAKE_PROJECT_NAME}]")
else()
message(STATUS "This project is a top-level one")
endif()
add_library(${PROJECT_NAME} STATIC)

find_package(cpprestsdk REQUIRED)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/FormatRequest.cpp
        src/TimeConversions.cpp
        src/FundamentalData.cpp
        src/PriceData.cpp
        src/TechnicalData.cpp
        src/NewsData.cpp
        src/MacroData.cpp
)

target_link_libraries(${PROJECT_NAME} 
    cpprestsdk::cpprest
    spdlog
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/FinancialDataConfig.h" DESTINATION include)

# test logic
option(TEST_FINNHUB "Test library for finnhub endpoints" OFF)
option(TEST_FMP "Test library for fmp endpoints" ON)

configure_file(FinancialDataConfig.h.in FinancialDataConfig.h)

include(CTest)

if (TEST_FINNHUB OR TEST_FMP)
    add_subdirectory(tests)
endif()

