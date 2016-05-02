#pragma once

KAI_BEGIN

struct Process
{
	bool Failed;
	std::string Error;

	Process() : Failed(false) { }

protected:
	bool Fail(const char *fmt, ...);
	bool Fail(const std::string &);
};

KAI_END
