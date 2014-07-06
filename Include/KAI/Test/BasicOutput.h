
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#if defined(KAI_UNIT_TESTS) && !defined(KAI_TEST_BASIC_OUTPUT_H)
#	define KAI_TEST_BASIC_OUTPUT_H

KAI_TEST_BEGIN

/// Writes output in human-readable form
struct BasicOutput : IOutput
{
protected:
	Summary summary;
	bool verbose;

public:
	BasicOutput(bool V = true) : verbose(V) { }

	const Summary &GetSummary() const { return summary; }
	Summary GetSummary2() const { return summary; }

	// implementation of IOutput
	void RunBegin() { }
	void RunEnd() { }
	void MethodBegin(const String &);
	void MethodEnd(const String &);
	void SuiteBegin(const String &);
	void SuiteEnd(const String &);
	void ModuleBegin(const String &);
	void ModuleEnd(const String &);
	void NewResult(const Result &);
	void NewException(const Exception::Base &);
	void NewStdException(const std::exception &);
	void NewUnknownException();
	void Run(Module *);
	void SetVerbose(bool V) { verbose = V; }

	// ends

	void Write(const Result &R);

	static void Register(Registry &);
};

KAI_TEST_END

KAI_BEGIN

KAI_TYPE_TRAITS_DERIVED(Test::BasicOutput, Type::Number::TestOutputBasic, 0, Test::IOutput);

KAI_END

#endif // KAI_TEST_BASIC_OUTPUT_H

//EOF
