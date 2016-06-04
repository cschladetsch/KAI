#include "Main.h"
#include <Tau/TauParser.h>

using namespace std;
using namespace kai;
using namespace kai::tau;
typedef TauTokenEnumType Tok;

struct TauTest : ::testing::Test
{
 	bool Test1()
	{
		auto input = "namespace Foo\n\tclass Bar\n\t\tint Method()\n";
        Registry r;
        TauLexer l(input, r);
        l.Process();
        if (l.Failed)
            cerr << l.Error << endl;

        TauTokenEnumType::Enum res[] = {
                Tok::Namespace, Tok::Whitespace, Tok::Ident, Tok::NewLine,
                Tok::Tab, Tok::Class, Tok::Whitespace, Tok::Ident, Tok::NewLine,
                Tok::Tab, Tok::Tab, Tok::Ident, Tok::Whitespace, Tok::Ident, Tok::OpenParan,Tok::CloseParan, Tok::NewLine };

        return true;
	}
	virtual void SetUp() override
	{
	}

	virtual void TearDown() override
	{
	}
};

TEST_F(TauTest, TestLex1)
{

}

