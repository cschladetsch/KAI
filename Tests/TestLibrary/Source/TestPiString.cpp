#include "TestBase.h"
#include "KAI/Translator/Language.h"

KAI_BEGIN

TEST(TestStrings, TestConcat)
{
	Console console;
	Process::trace = 1;
	console.SetLanguage((int)Language::Pi);

	console.Execute("\"hello\"");
	ASSERT_EQ(ConstDeref<String>(console.GetExecutor()->GetDataStack()->Top()), String("hello"));

	console.Execute("\"hello,\" \" world\" +");
	ASSERT_EQ(ConstDeref<String>(console.GetExecutor()->GetDataStack()->Top()), String("hello, world"));
}

KAI_END

