#pragma once

#include "KAI/Core/BuiltinTypes.h"
#include "KAI/Core/Console.h"
#include "KAI/Core/Executor.h"

#include <gtest/gtest.h>

KAI_BEGIN

struct KAITestClass : ::testing::Test
{
	Registry reg;
	Tree tree;

	void SetUp();// override;
	void TearDown();// overide;

	virtual void AddrequiredClasses();
};

KAI_END
