CppUT
=====

[![Build Status](https://travis-ci.org/murrekatt/cpput.svg?branch=master)](https://travis-ci.org/murrekatt/cpput)

The goal of this unit testing framework is to make it easy and clear how to
effectively write good unit tests in C++. The framework is inspired by Google
Test and CppUnitLite. It comes as a single header file for simplicity.


Getting Started
---------------

Simply include the test framework header file to get started access to the
testing framework and test macros.

	#include <cpput/TestHarness.hpp>

Nothing else is necessary, so let's look at a simple example.

	[Test_MyClass.cpp]

	#include <cpput/TestHarness.hpp>
	#include <MyClass.hpp>

	TEST(MyClass, some_descriptive_name)
	{
		// Arrange
		MyClass m;
		// Act
		m.foo();
		// Assert
		ASSERT_TRUE(m.isFoo());
	}

	CPPUT_TEST_MAIN;

The above example defines a test for MyClass and can be compiled as is into a
runnable test binary. Let's look at the important parts.

First, the macro TEST(group, name) defines a new test and automatically 
registers itself with the testing framework. A good convention here is to 
use the name of the class or similar as the group and the a descriptive name 
of what the test does and expects. Descriptive means that the person reading
it should understand what the test does and how that translates into how the
class functions.

Second, the body of the test should be short, clear and to the point of what
is being tested. A good practice is to use AAA (Arrange, Act, Assert), which
makes it clear what is the initial state, what is being done and lastly, what
is tested for.

Lastly, the helper macro `CPPUT_TEST_MAIN` that defines a proper main that runs
the test framework and all automatically registered tests. In most non-trivial 
cases you'll want to not put the macro in the same file as your tests, but 
rather have a separate main.cpp with this defined.

	[main.cpp]

	#include <cpput/TestHarness.hpp>

	CPPUT_TEST_MAIN;

For larger test suits, it's recommended to group the tests per class in separate
files and let the compiler combine them into a single executable.


Test Macros
-----------

Tests can be declared in two ways depending on what is needed. Let's have a 
look at each way.

	TEST(group, name)

This is the simplest way to define a test. just give it a group and a name
and it'll automatically register itself to be run by the testing framework.

	TEST_FIXTURE(fixture, name)

For more advanced tests it can be a good idea to use a fixture to setup the
initial state before running the test. Also if multiple tests share the same
initial state it's good to use a fixture. This fixture is given as a class or
struct as the first argument.

An example of using a test fixture looks like so.

	[Test_MyClass.cpp]

	#include <cpput/TestHarness.hpp>
	#include <MyClass.hpp>

	namespace
	{
		struct MyClassFixture
		{
			MyClass m_;
		};
	}

	TEST_FIXTURE(MyClassFixture, some_descriptive_name)
	{
		// Arrange
		// Act
		m_.foo();
		// Assert
		ASSERT_TRUE(m_.isFoo());
	}

	CPPUT_TEST_MAIN;

This declares the fixture which creates the initial state (in this case the
MyClass instance), which then is available in the test body.


Assert Macros
-------------

To assert conditions in tests there are six macros that can be used. Each is
explained below.

	ASSERT_TRUE(expression)
	ASSERT_FALSE(expression)

These macros check the given expression's validity. Any expression is possible
and an example could be foo == bar.

	ASSERT_EQ(expected, actual)
	ASSERT_NEQ(expected, actual)

These macros compare two integers for equality.

	ASSERT_STREQ(expected, actual)

This macro compares two C strings or equality.

	ASSERT_NEAR(expected, actual, epsilon)

This macro compares two floating-point numbers for equality, given an error
margin.


Naming
------

As with any other programming naming is essential. Most importantly, the name
of the test and the naming of the test files. Keep the class name part of the
test file name, e.g. Test_MyClass.cpp. This clearly tells where MyClass tests
are. Most importantly, the name of the tests should be describing what they 
are testing and even mention about the high-level outcome. For example, like
this:

	TEST(MyClass, throws_exception_if_not_initialized_before_doing_something)
	{
		// Arrange
		MyClass m;
		// Act
		try
		{
			m.doSomething();
			// Assert
			ASSERT_TRUE(false);
		}
		catch (const std::exception&)
		{
		}
	}

This is a test to see that there is an exception if the instance is not
explicitly initialized before calling the method doSomething.


Custom Result Writer
--------------------

If you need to get a special output from the tests you can customize
your own results by deriving from `ResultWriter`. Then you need to
run the tests like this:

    int main()
    {
      CustomResultWriter writer;
      return ::cpput::Runner::run(writer);
    }


Examples
--------

To build the examples just:

    cd examples
    make

and then run

    ./unittests

Contribution
------------

Please report any bugs or features using Github issues or send a pull request.
