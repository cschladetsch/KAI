#pragma once

#include "KAI/Core/BuiltinTypes.h"
#include "KAI/Core/Console.h"
#include "KAI/Core/Executor.h"
#include "KAI/Core/Object/GetStorageBase.h"
#include "KAI/Core/StringStreamTraits.h"

#include <gtest/gtest.h>

KAI_BEGIN

// TODO: Rename to BaseTestClass once refactoring works with VSCode again :P
class KAITestClass : public ::testing::Test
{
protected:
	// TODO: could use PIMPL here, but compile times don't seem to matter for tests so far
	Registry reg;
	Tree tree;

protected:
	void SetUp();
	void TearDown();

	virtual void AddrequiredClasses();
};

KAI_END
