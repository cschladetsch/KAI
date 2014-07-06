
#include "KAI/KAI.h"
#include "KAI/Test/Base.h"
#ifdef KAI_UNIT_TESTS
#	include "KAI/BuiltinTypes/Bool.h"

KAI_TEST_BEGIN

void Module::Pre()
{
}

void Module::Post()
{
}

void Module::Create()
{
}

bool Module::Destroy()
{
	return true;
}

Object UpCast(Object& Q)
{
	return Q.GetClass()->UpCast(Q.GetStorageBase());
}

Object Module::Run(bool verbose)
{
	BasicOutput out(verbose);
	Run(&out);
	return New(out.GetSummary());
}

Object Module::Run(Object output)
{
	Object base = UpCast(output);
	IOutput *out = Deref<IOutput *>(base);
	Run(out);
	Object summary = output.New<Summary>(out->GetSummary());
	return summary;
}

void Module::Run(IOutput *output)
{
	output->ModuleBegin(Name);
	Suites::Types::const_iterator A = suites.GetTypes().begin(), B = suites.GetTypes().end();
	for (; A != B; ++A)
	{
		SuiteBase *suite = 0;
		try
		{
			suite = (SuiteBase *)suites.Create(A->first);
			output->SuiteBegin(suite->Name);
			suite->Run(output);
		}
		catch (Exception::Base const &E)
		{
			output->NewException(E);
		}
		catch (std::exception const &E2)
		{
			output->NewStdException(E2);
		}
		catch (...)
		{
			output->NewUnknownException();
		}
		output->SuiteEnd(suite->Name);
		delete suite;
	}
	output->ModuleEnd(Name);
}

void Module::Register(Registry &R, const char *name)
{
	Object (Module::*Run)(Object) = &Module::Run;	// used to disambiguate the overloaded method name
	Object (Module::*RunAll)(bool) = &Module::Run;	// used to disambiguate the overloaded method name
	ClassBuilder<Module>(R, name)
		.Methods
		("Run", Run)
		("RunAll", RunAll)
		;
}

void Summary::Register(Registry &R)
{
	ClassBuilder<Summary>(R, "Summary")
		;
}

KAI_TEST_END

#endif // KAI_UNIT_TESTS

//EOF
