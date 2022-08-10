enable_testing()

if (TEST_FINNHUB)
    add_subdirectory(finnhub)
endif()

if (TEST_ALPHA)
    add_subdirectory(alphavantage)
endif()