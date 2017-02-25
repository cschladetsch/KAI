#include "TestCommon.h"
#include <boost/filesystem.hpp>

USING_NAMESPACE_KAI

namespace fs = boost::filesystem;
using namespace std;

std::list<kai::Language> TestLanguages;

TEST(TestLangScripts, TestAll)
{
	for (auto lang : TestLanguages)
	{
		const string name = ToString(lang);
		const string extension = "." + name;

		fs::path scripts = fs::system_complete("../../Source/Test/Scripts/" + name);

		fs::directory_iterator file(scripts);
		fs::directory_iterator end;
		TEST_COUT << "Testing language: " << name;

		for (; file != end; ++file)
		{
			try
			{
				if (file->path().extension() == extension)
				{
					Console console;
					console.SetLanguage(lang);
					console.GetExecutor()->SetTraceLevel(10);

					const char *script = file->path().string().c_str();
					TEST_COUT << "...Now running script: " << file->path().filename().string().c_str() << "\n";

					EXPECT_TRUE(console.ExecuteFile(script));
				}
			}
			catch (Exception::Base &e)
			{
				TEST_CERR << "Exception: " << e.ToString().c_str();// << endl;
			}
		}
	}
}
