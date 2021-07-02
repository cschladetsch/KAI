#include <cwctype>
#include <boost/filesystem.hpp>

#include "TestCommon.h"
#include "TestLangCommon.h"

#include <boost/algorithm/string/predicate.hpp>

#include "File.h"

KAI_BEGIN

void ToLower(std::string &str)
{
    KAI_NOT_IMPLEMENTED();
}

std::wstring ToLower(std::wstring const &str)
{
    std::wstring result;
    result.resize(str.size());
    std::transform(str.begin(), str.end(), result.begin(), std::towlower);
    return result;
}

void ToLower(std::wstring &str)
{
    std::transform(str.begin(), str.end(), str.begin(), std::towlower);
}

using namespace std;
namespace fs = boost::filesystem;

void TestLangCommon::SetUp()
{
    _reg = &_console.GetRegistry();
    _exec = &*_console.GetExecutor();
    _data = &*_exec->GetDataStack();
    _context = &*_exec->GetContextStack();
    _tree = &_console.GetTree();
    _root = _tree->GetRoot();
}

void TestLangCommon::TearDown()
{
}

void TestLangCommon::ExecScripts()
{
    const fs::path scriptsRoot(KAI_STRINGISE(KAI_SCRIPT_ROOT));
#if 0//def BOOST_OS_WINDOWS
    const auto ext = File::Extension(KAI_STRINGISE_WIDE(".pi"));
#else
    const auto ext = File::Extension(".pi");
#endif
    _console.SetLanguage(Language::KAI_LANG_NAME);
    for (auto const &scriptName : File::GetFilesWithExtensionRecursively(scriptsRoot, ext))
    {
        if (boost::algorithm::contains(scriptName.c_str(), "WIP"))
        {
            KAI_TRACE() << "Skipping script: " << scriptName.generic_string().c_str();
            continue;
        }
        KAI_TRACE() << "Testing script: " << scriptName.generic_string().c_str();
        auto contents = File::ReadAllText(scriptName);
        _console.Execute(contents.c_str());
    }
}

KAI_END

//EOF
