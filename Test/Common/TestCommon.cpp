#include "TestCommon.h"

#include <KAI/Core/BuiltinTypes/All.h>
#include <KAI/Core/Tree.h>

#include "KAI/Core/Console.h"
#include "KAI/Core/StringStream.h"

KAI_BEGIN

void TestCommon::SetUp() {
    _reg = new Registry();
    _tree = new kai::Tree();

    _reg->AddClass<void>();
    _reg->AddClass<bool>();
    _reg->AddClass<int>();
    _reg->AddClass<float>();
    _reg->AddClass<String>();
    _reg->AddClass<StringStream>();
    _reg->AddClass<BinaryStream>();

    _root = _reg->New<void>();
    _tree->SetRoot(_root);
    _reg->SetTree(*_tree);

    AddRequiredClasses();
}

Registry &TestCommon::Reg() const { return *_reg; }

Tree &TestCommon::GetTree() const { return *_tree; }

Object TestCommon::Root() const { return _root; }

void TestCommon::TearDown() {
    delete _reg;
    delete _tree;
}

KAI_END
