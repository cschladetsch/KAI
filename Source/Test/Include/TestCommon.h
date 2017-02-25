#pragma once

#include "KAI/Core/BuiltinTypes.h"
#include "KAI/Core/Console.h"
#include "KAI/Core/Executor.h"
#include "KAI/Core/Object/GetStorageBase.h"
#include "KAI/Core/StringStreamTraits.h"

#include <gtest/gtest.h>

KAI_BEGIN

// TODO: Rename to BaseTestClass once refactoring works with VSCode again :P
class TestCommon : public ::testing::Test
{
protected:
	Registry *_reg;
	Tree *_tree;
	Object _root;

protected:
	Registry &Reg();
	Tree &Tree();
	Object Root() const;

protected:
	void SetUp();
	void TearDown();

	virtual void AddrequiredClasses();
};

KAI_END

// HACK to allow printing for tests. see http://stackoverflow.com/questions/16491675/how-to-send-custom-message-in-google-c-testing-framework
namespace testing
{
 namespace internal
 {
  enum GTestColor {
      COLOR_DEFAULT,
      COLOR_RED,
      COLOR_GREEN,
      COLOR_YELLOW
  };

  extern void ColoredPrintf(GTestColor color, const char* fmt, ...);
 }
}

#define TEST_PRINTF(COLOR, ...)  do { testing::internal::ColoredPrintf(testing::internal::COLOR, "[          ] "); testing::internal::ColoredPrintf(testing::internal::COLOR, __VA_ARGS__); } while(0)

// C++ stream interface
class TestCout : public std::stringstream
{
public:
	bool _isError;
	TestCout(bool e = false) : _isError(e) { }
    ~TestCout()
    {
		if (_isError)
			TEST_PRINTF(COLOR_RED, "%s\n",str().c_str());
        else
			TEST_PRINTF(COLOR_YELLOW, "%s\n",str().c_str());
    }
};

#define TEST_COUT  TestCout()
#define TEST_CERR  TestCout(true)
