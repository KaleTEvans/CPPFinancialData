add_executable(test-FMP FMPTest.cpp)

target_link_libraries(test-FMP cpprestsdk::cpprest FinancialData)

add_test(NAME "test1-FMP" COMMAND "test-FMP" "AAPL" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
