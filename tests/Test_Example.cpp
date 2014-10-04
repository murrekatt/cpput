#include <TestHarness.hpp>

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
