#include <KAI/Core/Config/Base.h>
#include <string>
#include <vector>

KAI_BEGIN

typedef char byte;

std::string ReadTextFile(const char *fileName);
std::vector<byte> ReadBinaryFile(const char *fileName);

bool WriteTextFile(const char *contents, const char *fileName);
bool WriteBinaryFile(const void *contents, const size_t len, const char *fileName);
bool WriteBinaryFile(std::vector<byte> const &, const char *fileName);

KAI_END

