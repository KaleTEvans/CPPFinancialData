enable_testing()

if (TEST_FINNHUB)
    add_subdirectory(finnhub)
endif()

if (TEST_FMP)
    add_subdirectory(FMP)
endif()
