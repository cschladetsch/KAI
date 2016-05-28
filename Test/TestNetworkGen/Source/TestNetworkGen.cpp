#include "Main.h"
#include "KAI/Core/Registry.h"
#include "Tau/TauLexer.h"

#include <array>

using namespace std;
using namespace kai;
using namespace kai::tau;

struct MyTest : ::testing::Test
{
	virtual void SetUp() override
	{
	}

	virtual void TearDown() override
	{
	}
};

typedef TauTokenEnumType Tok;

//template <class Cont, class Fun>
//std::vector<typename Cont::value_type> filter(Cont const &t, Fun f)
//{
//	std::vector<typename Cont::value_type> result;
//	for (const auto &x : t)
//		result.emplace_back(f(x));
//	return std::move(result);
//}

TEST_F(MyTest, Test)
{
	auto input = "namespace Foo\n\tclass Bar\n\t\tint Method()";
	Registry r;
	TauLexer l(input, r);
	l.Process();
	if (l.Failed)
		cerr << l.Error << endl;
	ASSERT_FALSE(l.Failed);

	TauTokenEnumType::Enum res[] = { Tok::Namespace, Tok::Ident, Tok::Class, Tok::NewLine,Tok::Tab,Tok::Tab,Tok::Ident,Tok::OpenParan,Tok::CloseParan };
	int n = 0;
	for (auto t : l.GetTokens())
		ASSERT_EQ(t.type, res[n]);
}


