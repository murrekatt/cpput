#include <TestHarness.hpp>

TEST(assert_ASSERT_TRUE, simple_expressions_pass)
{
  ASSERT_TRUE(true);
  ASSERT_TRUE(!false);
  ASSERT_TRUE(1 == 2);
  ASSERT_TRUE(2 != 5);
  ASSERT_TRUE(1 < 10);
  ASSERT_TRUE(100 > 2.34);
  ASSERT_TRUE(10 > 2);
  ASSERT_TRUE(1.123  < 3.14);
  ASSERT_TRUE("foo" == "foo");
  ASSERT_TRUE("ciphertext" != "plaintext");
}

namespace
{

struct Example
{
  int value() { return 42; }
};

struct ExampleFixture
{
  Example e_;
};

} // namespace

TEST_F(ExampleFixture, when_doing_this_that_will_happen_as_a_result)
{
  // Arrange
  // Act
  // Assert
  ASSERT_EQ(42, e_.value());
}

/////////////////////////////////////////////////////////////////////////////

TEST(Example, simple_test_of_something_that_should_result_in_something)
{
  // Arrange
  Example e;
  // Act
  int value = e.value();
  // Assert
  ASSERT_EQ(42, value);
}
