add_executable(test-finnhub tester.cpp)

target_link_libraries(test-finnhub cpprestsdk::cpprest FinancialData)

add_test(NAME "test1-finnhub" COMMAND "test-finnhub" "AAAA" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})