
#include "KAI/KAI.h"
#include "KAI/Test/Base.h"
#ifdef KAI_UNIT_TESTS
#	include "KAI/BuiltinTypes/Bool.h"
#	include <iostream>

KAI_TEST_BEGIN

XmlOutput::XmlOutput()
{
}

void XmlOutput::RunBegin()
{
}

void XmlOutput::RunEnd()
{
}

void XmlOutput::MethodBegin(const String &name)
{
	xml << "<Method name='" << name << "'>";
}

void XmlOutput::MethodEnd(const String &)
{
	xml << "</Method>";
}

void XmlOutput::SuiteBegin(const String &name)
{
	xml << "<Suite name='" << name << "'>";
}

void XmlOutput::SuiteEnd(const String &)
{
	xml << "</Suite>";
}

void XmlOutput::ModuleBegin(const String &name)
{
	xml << "<TestRun>";

	summary = Summary();
	summary.module_name = name;

	xml << "<Module name='" << name << "'>";
}

void XmlOutput::ModuleEnd(const String &)
{
	xml << "</Module>";
	xml << "</TestRun>";
}

void XmlOutput::NewResult(const Result &R)
{
	BasicOutput::NewResult(R);
	R.Write(xml);
}

void XmlOutput::NewException(const Exception::Base &E)
{
	summary.num_exceptions++;
	if (verbose)
		std::cout << E.ToString() << std::endl;
	xml << "<Exception>" << E.ToString().c_str() << "</Exception>";
}

void XmlOutput::NewStdException(const std::exception &E)
{
	summary.num_exceptions++;
	if (verbose)
		std::cout << "std::exception: " << E.what() << std::endl;
	xml << "<StdException>" << E.what() << "</StdException>";
}

void XmlOutput::NewUnknownException()
{
	summary.num_exceptions++;
	if (verbose)
		std::cout << "UnknownException" << std::endl;
	xml << "<UnknownException />";
}

void XmlOutput::Run(Module *)
{
}

void XmlOutput::Write(const Result &)
{
}

void XmlOutput::Register(Registry &R, const char *N)
{
	// TODO TESTS
	//ClassBuilder<XmlOutput>(R, N)
	//	.Methods
	//		("SetVerbose", &XmlOutput::SetVerbose)
	//		("RunBegin", &XmlOutput::RunBegin)
	//		("RunEnd", &XmlOutput::RunEnd)
	//		("ToString", &XmlOutput::ToString)
	//		("GetSummary", &XmlOutput::GetSummary2)
	//	;
}

KAI_TEST_END

#endif

//EOF
