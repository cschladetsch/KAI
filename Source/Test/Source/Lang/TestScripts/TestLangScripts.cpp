#include "TestCommon.h"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
using namespace std;
using namespace kai;

std::list<kai::Language> TestLanguages;
fs::path ScriptRoot = "../../Source/Test/Scripts/";

// single test that is used to run all scripts for all languages, configured
// via global TestLanguages.
TEST(TestLangScripts, TestAll)
{
	for (auto lang : TestLanguages)
	{
		const string name = ToString(lang);
		TEST_COUT << "Testing language: " << name;

		const string extension = "." + name;

		fs::path dir = ScriptRoot;
		fs::directory_iterator script(dir.append(extension)), end;

		for (; script != end; ++script)
		{
			if (script->path().extension() != extension)
				continue;

			Console console;
			console.SetLanguage(lang);
			console.GetExecutor()->SetTraceLevel(10);
			const char *source = script->path().string().c_str();
			TEST_COUT << "...Now running script: " << script->path().filename().string().c_str() << "\n";

			try
			{
				EXPECT_TRUE(console.ExecuteFile(source));
			}
			catch (Exception::Base &e)
			{
				TEST_CERR << "Exception: " << e.ToString().c_str();// << endl;
			}
		}
	}
}
