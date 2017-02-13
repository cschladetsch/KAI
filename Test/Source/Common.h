#pragma once

#include "KAI/Core/BuiltinTypes.h"
#include "KAI/Core/Console.h"
#include "KAI/Core/Executor.h"

#include <gtest/gtest.h>

KAI_BEGIN

// TODO: Rename to BaseTestClass once refactoring works with VSCode again :P
class KAITestClass : public ::testing::Test
{
	// TODO: could use PIMPL here, but compile times don't seem to matter for tests so far
protected:
	Registry reg;
	Tree tree;

protected:
	void SetUp();
	void TearDown();

	virtual void AddrequiredClasses();
};

KAI_END
