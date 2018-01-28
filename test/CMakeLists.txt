set(CPPFILES
	ListTestDriver.cpp
  )

set(PRIVATE_HFILES  
  )

include_directories(${PROJECT_SOURCE_DIR}/test)

add_executable(StarCounterPGTest ${CPPFILES} ${PRIVATE_HFILES})
target_link_libraries (StarCounterPGTest gtest gtest_main)

set_property(TARGET StarCounterPGTest PROPERTY FOLDER "${STARCOUNTERPG_PREFIX}test")

add_test(NAME StarCounterPGTest COMMAND StarCounterPG)
