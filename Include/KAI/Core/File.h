#pragma once

#include "KAI/Core/Config/Base.h"
#include <boost/filesystem.hpp>
#include <string>
#include <vector>

KAI_BEGIN

namespace File
{
    using Pathname = boost::filesystem::path;
    using Extension = Pathname::string_type;
    using Pathnames = std::vector<Pathname>;
    using byte = std::uint8_t;

    bool Exists(Pathname const &);
    bool IsFile(Pathname const &);
    bool IsFolder(Pathname const &);

    // return a vector of all files that have the given extension, starting from root, recursively
    Pathnames GetFilesWithExtensionRecursively(Pathname const &root, Extension const &ext);
    Pathnames GetFilesRecursively(Pathname const &root);
    Pathnames GetFiles(Pathname const &root);

    // Read all text from given filename
    std::string ReadAllText(Pathname const &);
    std::wstring ReadAllWideText(Pathname const &);

    // replace a file with the contents of a string
    bool ReplaceWithText(std::string const &, Pathname const&);

    // read all bytes in a file
    template <class Byte = std::uint8_t>
    std::vector<Byte> ReadAllBinary(Pathname const &path)
    {
        using namespace std;
        namespace fs = boost::filesystem;

        fs::basic_ifstream<Byte> in(path, ios_base::binary | ios_base::in);
        in.seekg(0, ios_base::end);
        const auto len = in.tellg();
        in.seekg(0, ios_base::beg);
        vector<Byte> contents(len);
        in.read(&contents[0], len);
        return contents;
    }

    // replace a file with a sequence of bytes
    bool ReplaceWithBinary(byte const *, size_t num_bytes, Pathname const &);

    inline bool ReplaceWithBinary(std::vector<byte> const &bytes, Pathname const &pathname)
    {
        return ReplaceWithBinary(&*bytes.begin(), bytes.size(), pathname);
    }
}

KAI_END

