#include "TestCommon.h"
#include <cstdarg>

KAI_BEGIN

void TestCommon::SetUp()
{
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

    AddrequiredClasses();
}

Registry &TestCommon::Reg()
{
    return *_reg;
}

Tree &TestCommon::GetTree()
{
    return *_tree;
}

Object TestCommon::Root() const
{
    return _root;
}

void TestCommon::AddrequiredClasses()
{
}

void TestCommon::TearDown()
{
    delete _reg;
    delete _tree;
}

KAI_END

namespace testing::internal
{
    //void ColoredPrintf(GTestColor color, const char* fmt, ...)
    //{
    //     va_list args;
    //    va_start(args, fmt);
    //    char buffer[1000];
    //    vsprintf(buffer, fmt, args);
    //    printf("%s\n", buffer);
    //}
}

