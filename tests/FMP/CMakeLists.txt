add_executable(test-fundamental test-fundamental.cpp)
add_executable(test-macro test-macro.cpp)
add_executable(test-market test-market.cpp)
add_executable(test-news test-news.cpp)
add_executable(test-price test-price.cpp)
#add_executable(test-technical test-technical.cpp)

target_link_libraries(test-fundamental cpprestsdk::cpprest FinancialData)
target_link_libraries(test-macro cpprestsdk::cpprest FinancialData)
target_link_libraries(test-market cpprestsdk::cpprest FinancialData)
target_link_libraries(test-news cpprestsdk::cpprest FinancialData)
target_link_libraries(test-price cpprestsdk::cpprest FinancialData)
#target_link_libraries(test-technical cpprestsdk::cpprest FinancialData)

#add_test(NAME "test-fundamental" COMMAND "test-fundamental" "NVDA" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
#add_test(NAME "test-macro" COMMAND "test-macro" "NVDA" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
#add_test(NAME "test-market" COMMAND "test-market" "NVDA" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
#add_test(NAME "test-news" COMMAND "test-news" "NVDA" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME "test-price" COMMAND "test-price" "NVDA" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
#add_test(NAME "test-technical" COMMAND "test-technical" "NVDA" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
