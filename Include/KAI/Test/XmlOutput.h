

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#if !defined(KAI_TEST_XML_OUTPUT_H) && defined(KAI_UNIT_TESTS)
#	define KAI_TEST_XML_OUTPUT_H

KAI_TEST_BEGIN

/// Writes output in human-readable form
struct XmlOutput : BasicOutput
{
	StringStream xml;

	XmlOutput();

	// implementation of IOutput
	void RunBegin();
	void RunEnd();
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

	Summary GetSummary2() const { return summary; }
	String ToString() const { return xml.ToString(); }
	void Write(const Result &R);
	void SetVerbose(bool B) { verbose = B; }

	static void Register(Registry &, const char * = "TestOutputXml");
};

KAI_TEST_END

KAI_BEGIN

StringStream &operator<<(StringStream &, const Test::XmlOutput &);

//KAI_TYPE_TRAITS_DERIVED(Test::XmlOutput, Type::Number::TestOutputXml, Type::Properties::StringStreamInsert, Test::IOutput);
KAI_TYPE_TRAITS(Test::XmlOutput, Type::Number::TestOutputXml, Type::Properties::StringStreamInsert)

KAI_END

#endif // KAI_TEST_XML_OUTPUT_H

//EOF
