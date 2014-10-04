#ifndef CPP_UNIT_TEST_H_
#define CPP_UNIT_TEST_H_

///////////////////////////////////////////////////////////////////////////////
///
/// \brief    A light-weight and easy to use unit testing framework for C++
/// \details  Header-only unit testing framework that makes unit testing easy
///           and quick to set up.
/// \version  0.1.0
/// \date     December 2011
/// \author   Tommy Back
///
///
/// Simply include this header file to get started.
///
/// \code
/// #include <cpput/TestHarness.h>
/// #include <Foo.h>
///
/// TEST(Foo, some_descriptive_name)
/// {
///   // Arrange
///   Foo foo;
///   // Act
///   bool result = foo.isBar();
///   // Assert
///   ASSERT_TRUE(result);
/// }
/// \endcode
///
/// In case you want to keep a single file with tests that compile to an
/// executable you can also add the main function to the end of the file.
/// This is simple to do with the provided macro:
///
/// \code
/// DECLARE_TEST_MAIN_FUNCTION;
/// \endcode
///
/// For larger test suits, it's recommended to group the tests per class
/// in separate files and let the compiler combine them into a single
/// executable which has a main.cpp file that only has the main function
/// declared.
///
/// \example
/// [Test_Foo.cpp]
///
/// \code
/// #include <cpput/TestHarness.h>
/// #include <Foo.h>
///
/// TEST(Foo, foo_bar_z)
/// {
///   ...
/// \endcode
///
/// [main.cpp]
///
/// \code
/// #include <cpput/TestHarness.h>
///
/// DECLARE_TEST_MAIN_FUNCTION;
/// \endcode
///
///

#include <ctime>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>

namespace test
{
namespace unit
{

struct Failure
{
  Failure(const std::string& filename, std::size_t line, const std::string& message)
    : type_("Failure") // TODO: type of failure?
    , filename_(filename)
    , line_(line)
    , message_(message)
  {
  }
  
  friend std::ostream& operator<<(std::ostream& os, const Failure& f)
  {
    os << f.filename_ << ", line " << f.line_ << ": " << f.message_;
    return os;
  }

  std::string type_;
  std::string filename_;
  std::size_t line_;
  std::string message_;
};

/////////////////////////////////////////////////////////////////////////////

struct IOutputter
{
  virtual ~IOutputter() {}
  
  virtual void startTest(const std::string& className, const std::string& name) = 0;
  virtual void endTest(bool success) = 0;
  
  virtual void failure(const Failure& failure) = 0;
  virtual int getNumberOfFailures() const = 0;
};

/////////////////////////////////////////////////////////////////////////////

class TextOutputter : public IOutputter
{
public:
  TextOutputter()
    : testCount_(0)
  {
  }

  virtual ~TextOutputter()
  {
    std::cout << "\n";
    if (failures_.empty())
    {
      std::cout << "All tests pass.\n";
      return;
    }
    // print details of all failures
    std::cout << "\n";
    for (std::size_t i=0; i<failures_.size(); ++i)
    {
      std::cout << failures_[i] << "\n";
    }
    std::cout << "\n" << failures_.size() << " out of " << testCount_ << " tests failed.\n";
  }

  virtual void startTest(const std::string& className, const std::string& name)
  {
    testCount_++;
  }

  virtual void endTest(bool success)
  {
    if (success)
      std::cout << '.';
    else
      std::cout << 'F';
  }

  virtual void failure(const Failure& failure)
  {
    failures_.push_back(failure);
  }

  virtual int getNumberOfFailures() const { return failures_.size(); }

private:
  int                  testCount_;
  std::vector<Failure> failures_;
};

/////////////////////////////////////////////////////////////////////////////

class XmlOutputter : public IOutputter
{
public:
  XmlOutputter()
    : startTime_(0)
  {
    std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    std::cout << "<testsuite>\n";
  }

  virtual ~XmlOutputter()
  {
    std::cout << "</testsuite>\n";
  }

  virtual void startTest(const std::string& className, const std::string& name)
  {
    startTime_ = std::clock();
    std::cout << "  <testcase classname=\""
              << className
              << "\" name=\""
              << name
              << "\" time=\"";
  }

  virtual void endTest(bool success)
  {
    if (success)
    {
      std::cout << static_cast<float>(std::clock()-startTime_)/CLOCKS_PER_SEC << "\"";
      std::cout << "/>\n";
    }
    else
      std::cout << "  </testcase>\n";
  }

  virtual void failure(const Failure& failure)
  {
    std::cout << static_cast<float>(std::clock()-startTime_)/CLOCKS_PER_SEC << "\"";
    std::cout << ">\n"
              << "    <failure type=\""
              << failure.type_
              << "\">"
              << failure.message_
              << " in "
              << failure.filename_
              << ", line "
              << failure.line_
              << "</failure>\n";
    failureCount_++;
  }

  virtual int getNumberOfFailures() const
  {
    return failureCount_;
  }

private:
  std::clock_t startTime_;
  int          failureCount_;
};

/////////////////////////////////////////////////////////////////////////////

struct Result
{
  Result(const std::string& testClassName,
         const std::string& testName,
         IOutputter& out)
    : out_(out)
    , pass_(true)
  {
    out_.startTest(testClassName, testName);
  }
  
  ~Result()
  {
    out_.endTest(pass_);
  }

  template <typename T, typename U>
  void addFailure(const char* filename,
                  std::size_t line,
                  T expected,
                  U actual)
  {
    pass_ = false;
    std::stringstream ss;
    ss << std::setprecision(20)
       << "failed comparison, expected " << expected
       << " got " << actual << "\n";
    Failure f(filename, line, ss.str());
    out_.failure(f);
  }

  void addFailure(const char* filename,
                  std::size_t line,
                  const char* message)
  {
    pass_ = false;
    Failure f(filename, line, message);
    out_.failure(f);
  }

  IOutputter& out_;
  bool        pass_;
};

/////////////////////////////////////////////////////////////////////////////

class Repository;

class Case
{
  friend class Repository;

public:
  Case(const char* className, const char* name);
  virtual ~Case() {}

  void run(IOutputter& out)
  {
    Result result(test_unit_class_name_, test_unit_name_, out);
    try
    {
      do_run(result);
    }
    catch (const std::exception& e)
    {
      result.addFailure(__FILE__, __LINE__, std::string("Unexpected exception: ").append(e.what()).c_str());
    }
    catch (...)
    {
      result.addFailure(__FILE__, __LINE__, "Unspecified exception!");
    }
  }
  
  Case* next() { return test_unit_next_; }

private:
  virtual void do_run(Result& testResult_) = 0;

private:
  std::string test_unit_class_name_;
  std::string test_unit_name_;
  Case*       test_unit_next_;
};

/////////////////////////////////////////////////////////////////////////////

class Repository
{
public:
  static Repository& instance()
  {
    static Repository repo;
    return repo;
  }
  
  void add(Case* tc)
  {
    if (!cases_)
    {
      cases_ = tc;
      return;
    }

    // add as last
    Case* tmp = cases_;
    while (tmp->test_unit_next_)
      tmp = tmp->test_unit_next_;
    tmp->test_unit_next_ = tc;
  }
  
  Case* getCases() { return cases_; }

private:
  Repository() : cases_(0) {}
  Repository(const Repository& other);
  Repository& operator=(const Repository& rhs) const;

private:
  Case* cases_;
};

inline Case::Case(const char* className, const char* name)
  : test_unit_class_name_(className)
  , test_unit_name_(name)
  , test_unit_next_(0)
{
  Repository::instance().add(this);
}

/////////////////////////////////////////////////////////////////////////////

struct Runner
{
  static int run(bool xmlOutput=false)
  {
    if (xmlOutput)
    {
      XmlOutputter out;
      return do_run(out);
    }
    TextOutputter out;
    return do_run(out);
  }
  
private:
  static int do_run(IOutputter& out)
  {
    Case* c = Repository::instance().getCases();
    while (c)
    {
      c->run(out);
      c = c->next();
    }
    return out.getNumberOfFailures();
  }

private:
  Runner();
  Runner(const Runner& other);
  Runner& operator=(const Runner& rhs) const;
};

} // namespace unit
} // namespace test

// Convenience macro to get main function.
// TODO: check properly for -xmloutput
#define DECLARE_TEST_MAIN_FUNCTION \
int main(int argc, char* argv[]) { \
  return test::unit::Runner::run(argc > 1); \
}

///////////////////////////////////////////////////////////////////////////////
// Test Case Macros
///////////////////////////////////////////////////////////////////////////////

/// Stand-alone test case.
///
#define TEST(group,name) \
class group##name##Test : public test::unit::Case \
{ \
public: \
  group##name##Test() : test::unit::Case(#group,#name) {} \
  virtual ~group##name##Test() {} \
private: \
  virtual void do_run(test::unit::Result& testResult_); \
} group##name##TestInstance; \
inline void group##name##Test::do_run(test::unit::Result& testResult_)

/// Test case with fixture.
///
#define TEST_F(group,name) \
class group##name##FixtureTest : public group { \
public: \
    void do_run(test::unit::Result& testResult_); \
}; \
class group##name##Test : public test::unit::Case { \
public: \
    group##name##Test() : Case(#group,#name) {} \
    virtual void do_run(test::unit::Result& testResult_); \
} group##name##TestInstance; \
inline void group##name##Test::do_run(test::unit::Result& testResult_) { \
  group##name##FixtureTest test; \
  test.do_run(testResult_); \
} \
inline void group##name##FixtureTest::do_run(test::unit::Result& testResult_)

///////////////////////////////////////////////////////////////////////////////
// Assertion Macros
///////////////////////////////////////////////////////////////////////////////

#define ASSERT_TRUE(expression) \
{ \
  if (!(expression)) \
  { \
    testResult_.addFailure(__FILE__, __LINE__, #expression); \
    return; \
  } \
}

#define ASSERT_FALSE(expression) \
{ \
  if ((expression)) \
  { \
    testResult_.addFailure(__FILE__, __LINE__, #expression); \
    return; \
  } \
}

#define ASSERT_EQ(expected,actual) \
{ \
  if (!((expected) == (actual))) \
  { \
    testResult_.addFailure(__FILE__, __LINE__, expected, actual); \
    return; \
  } \
}

#define ASSERT_NEQ(expected,actual) \
{ \
  if (((expected) == (actual))) \
  { \
    testResult_.addFailure(__FILE__, __LINE__, expected, actual); \
    return; \
  } \
}

#define ASSERT_STREQ(expected,actual) { \
  if (!(std::string(expected) == std::string(actual))) \
  { \
    testResult_.addFailure(__FILE__, __LINE__, expected, actual); \
    return; \
  } \
}

#define ASSERT_NEAR(expected,actual,epsilon) \
{ \
  double actualTmp = actual; \
  double expectedTmp = expected; \
  double diff = expectedTmp - actualTmp; \
  if ((diff > epsilon) || (-diff > epsilon)) \
  { \
    testResult_.addFailure(__FILE__, __LINE__, expectedTmp, actualTmp); \
    return; \
  } \
}

#endif // CPP_UNIT_TEST_H_
