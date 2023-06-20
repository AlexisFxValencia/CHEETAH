
include(GoogleTest)

set(TEST_NAME test00)
add_executable(${TEST_NAME} test00.cpp test01.cpp test02.cpp Neutron_tests.cpp)

target_link_libraries(${TEST_NAME} CHEETAH_lib GTest::gtest_main)
target_include_directories(${TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_test(NAME ${TEST_NAME} COMMAND $<TARGET_FILE:${TEST_NAME}> --gtest_output=xml:junit.xml)
gtest_discover_tests(${TEST_NAME})
