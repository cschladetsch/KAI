#include <KAI/Core/File.h>
#include <KAI/Core/Object.h>

using namespace std;

KAI_BEGIN

string ReadTextFile(const char *fileName)
{
	KAI_NOT_IMPLEMENTED();
}

vector<byte> ReadBinaryFile(const char *fileName)
{
	KAI_NOT_IMPLEMENTED();
}

bool WriteTextFile(const char *contents, const char *fileName)
{
	KAI_NOT_IMPLEMENTED();
}

bool WriteBinaryFile(const void *contents, const size_t len, const char *fileName)
{
	KAI_NOT_IMPLEMENTED();
}

bool WriteBinaryFile(vector<byte> const &, const char *fileName)
{
	KAI_NOT_IMPLEMENTED();
}

KAI_END

