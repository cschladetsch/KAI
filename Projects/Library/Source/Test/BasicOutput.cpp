
#include "KAI/KAI.h"
#include "KAI/Test/Base.h"
#ifdef KAI_UNIT_TESTS
#	include <iostream>

KAI_TEST_BEGIN

void BasicOutput::MethodBegin(const String &N)
{
	if (verbose)
		std::cout << "test>    Method: Start " << N.c_str() << std::endl;
}

void BasicOutput::MethodEnd(const String &N)
{
	if (verbose)
		std::cout << "test>    Method: End " << N.c_str() << std::endl;
}

void BasicOutput::SuiteBegin(const String &N)
{
	if (verbose)
		std::cout << "    Suite: " << N.c_str() << std::endl;
}

void BasicOutput::SuiteEnd(const String &N)
{
	KAI_UNUSED(N);
		//std::cout << "test>  Suite: End " << N.c_str() << std::endl;
}

void BasicOutput::ModuleBegin(const String &N)
{
	//if (verbose)
		std::cout << "TestModule: " << N.c_str() << std::endl;
}

void BasicOutput::ModuleEnd(const String &N)
{
	//Summary::Failures::const_iterator A = summary.failures.begin(), B = summary.failures.end();
	//for (; A != B; ++A)
	//{
	//	Write(*A);
	//}
	if (verbose)
		std::cout << "test>Module: End " << N.c_str() << std::endl;
}

void BasicOutput::Write(const Result &R)
{
	nstd::string loc = R.location.ToString().c_str();//StdString();
	loc = loc.substr(loc.find_last_of('\\') + 1);
	StringStream text;
	text << "type=" << R.type.ToString() << ": outcome=" << R.outcome.ToString() << ": exp=" << R.expression;
	std::cout << loc.c_str() << text.ToString().c_str() << std::endl;

	debug::Trace(R.location) << text;

	//std::cout << "Result: " << R.outcome.ToString().c_str(); << std::endl;
	//std::cout << R.outcome.ToString().c_str() << ", ";// << std::endl;
}

void BasicOutput::NewResult(const Result &R)
{
	switch (R.outcome.type)
	{
	case Outcome::Failed: summary.num_failed++; break;
	case Outcome::Passed: summary.num_passed++; break;
	case Outcome::NotImplemented: summary.num_not_implemented++; break;
	}
	if (R.outcome == Outcome::Failed)
	{
		summary.failures.push_back(R);
		Write(R);
		if (verbose)
		{
			//std::cout << "test>      ! FAILED: ";
		}
	}
}

void BasicOutput::NewException(const Exception::Base &E)
{
	std::cout << "test>      ! EXCEPTION: " << E.ToString().c_str() << std::endl;
	++summary.num_exceptions;
}

void BasicOutput::NewStdException(const std::exception &E)
{
	std::cout << "test>      ! STD::EXCEPTION: " << E.what() << std::endl;
	++summary.num_exceptions;
}

void BasicOutput::NewUnknownException()
{
	std::cout << "test>      ! UNKNOWN EXCEPTION" << std::endl;
	++summary.num_exceptions;
}

void BasicOutput::Run(Module *module)
{
	module->Run(this);
}

void BasicOutput::Register(Registry &R)
{
	ClassBuilder<BasicOutput>(R, "TestOutput")
		.Methods
			("SetVerbose", &BasicOutput::SetVerbose)
			("GetSummary", &BasicOutput::GetSummary2)
		;
}

KAI_TEST_END

#endif // KAI_UNIT_TESTS

//EOF
