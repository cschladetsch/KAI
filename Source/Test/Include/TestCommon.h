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
