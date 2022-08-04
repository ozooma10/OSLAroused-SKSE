set(TEST_SUITE_NAME "${PROJECT_NAME}Tests")

find_package(GTest CONFIG REQUIRED)

file(GLOB TESTS_SOURCE *.h *.cpp)

ADD_EXECUTABLE("${TEST_SUITE_NAME}" ${TESTS_SOURCE})

target_compile_features(
	"${TEST_SUITE_NAME}"
	PRIVATE
		cxx_std_23
)

find_package(GTest CONFIG REQUIRED)
target_link_libraries("${TEST_SUITE_NAME}" "${PROJECT_NAME}" GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

add_test(NAME "${TEST_SUITE_NAME}" COMMAND "${TEST_SUITE_NAME}")