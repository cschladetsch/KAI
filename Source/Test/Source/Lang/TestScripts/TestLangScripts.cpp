#include "TestCommon.h"
#include <boost/filesystem.hpp>
#include "KAI/Core/File.h"
#include "KAI/Language/Common/Language.h"
#include <KAI/Console/Console.h>

std::list<kai::Language> TestLanguages;

using namespace boost::filesystem;
using namespace std;
using namespace kai;

KAI_BEGIN

path ScriptRoot = "../../Source/Test/Scripts/";

string LoadScriptText(const char *filename)
{
    return File::ReadAllText((ScriptRoot / path(filename)).generic_string().c_str());
}

KAI_END

// single test that is used to run all scripts for all languages, configured
// via global TestLanguages.
TEST(TestLangScripts, TestAll)
{
    for (auto lang : TestLanguages)
    {
        const string name = ToString(lang);
        TEST_COUT << ">>> Testing Language: " << name;

        const string extension = "." + name;
        path dir = ScriptRoot.append(name);
        directory_iterator file(dir), end;

        for (; file != end; ++file)
        {
            auto const &path = file->path();
            if (path.extension() != extension)
            {
                // TEST_COUT << "Ignoring " << path;
                continue;
            }

            Console console;
            console.SetLanguage(lang);
            console.GetCompiler()->SetTraceLevel(10);
//            console.GetExecutor()->SetTraceLevel(10);

            const char *source = path.string().c_str();
            TEST_COUT << "Running: " << path.filename().string().c_str();

            try
            {
                EXPECT_TRUE(console.ExecuteFile(source));
            }
            catch (Exception::Base &e)
            {
                // TEST_CERR 
                ASSERT_TRUE(false)<< "Exception: " << e.ToString().c_str();// << endl;
            }
        }
    }
}
