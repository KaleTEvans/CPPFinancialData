add_executable(test-alpha atest.cpp)

target_link_libraries(test-alpha cpprestsdk::cpprest FinancialData)

add_test(NAME "test1-alpha" COMMAND "test-alpha" "AAPL" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
