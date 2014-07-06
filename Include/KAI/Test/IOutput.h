
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#if !defined(KAI_TEST_IOUTPUT_H) && defined(KAI_UNIT_TESTS) 
#	define KAI_TEST_IOUTPUT_H

KAI_TEST_BEGIN

struct IOutput
{
	virtual ~IOutput() { }

	virtual void RunBegin() = 0;
	virtual void RunEnd() = 0;

	virtual void MethodBegin(const String &) = 0;
	virtual void MethodEnd(const String &) = 0;

	virtual void SuiteBegin(const String &) = 0;
	virtual void SuiteEnd(const String &) = 0;

	virtual void ModuleBegin(const String &) = 0;
	virtual void ModuleEnd(const String &) = 0;

	virtual void NewResult(const Result &) = 0;
	virtual void NewException(const Exception::Base &) = 0;
	virtual void NewStdException(const std::exception &) = 0;
	virtual void NewUnknownException() = 0;

	virtual void Run(Module *) = 0;
	virtual const Summary &GetSummary() const = 0;

	virtual void SetVerbose(bool) = 0;
};

KAI_TEST_END

KAI_BEGIN

KAI_TYPE_TRAITS(Test::IOutput *, Type::Number::TestOutput, 0);

KAI_END

#endif // KAI_TEST_IOUTPUT_H

//EOF
