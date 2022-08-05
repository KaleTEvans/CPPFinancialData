#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Financial::FinancialData" for configuration "Debug"
set_property(TARGET Financial::FinancialData APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Financial::FinancialData PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libFinancialDatad.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Financial::FinancialData )
list(APPEND _IMPORT_CHECK_FILES_FOR_Financial::FinancialData "${_IMPORT_PREFIX}/lib/libFinancialDatad.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
