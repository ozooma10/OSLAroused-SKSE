#include "TestHelper.h"

class ExampleTestOne : public ExampleTestBase {};

TEST_F(ExampleTestOne, HelloTest) {
    EXPECT_EQ("Hello", "World");
}
