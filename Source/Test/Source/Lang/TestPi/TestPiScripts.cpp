#include "TestCommon.h"
#include <boost/filesystem.hpp>

USING_NAMESPACE_KAI

namespace fs = boost::filesystem;


TEST(TestPiScripts, TestAll)
{
	fs::path scripts = fs::system_complete("../../Source/Test/Scripts/Pi");

	fs::directory_iterator file(scripts);
	fs::directory_iterator end;

	for (; file != end; ++file)
	{
		Console console;
		console.SetLanguage(Language::Pi);
		console.GetExecutor()->SetTraceLevel(10);

		const char *script = file->path().string().c_str();
		
		try
		{
			if (file->path().extension() == ".pi")
			{
				TEST_COUT << "Running script: " << file->path().filename().string().c_str() << "\n";
				ASSERT_TRUE(console.ExecuteFile(script));
			}
		}
		catch (Exception::Base &e)
		{
			TEST_CERR << "Exception: " << e.ToString().c_str();// << endl;
		}
	}
}
