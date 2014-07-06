
#include "KAI/KAI.h"
#include "KAI/Test/Base.h"
#ifdef KAI_UNIT_TESTS
#	include <iostream>

KAI_TEST_BEGIN

void SuiteBase::Pre()
{
}

void SuiteBase::Post()
{
}

String Outcome::ToString() const
{
	switch (type)
	{
	case None: return "None";
	case Passed: return "Passed";
	case Failed: return "Failed";
	case NotImplemented: return "NotImplemented";
	}
	return "UnknownOutcome";
}

String Operation::ToString() const
{
	switch (type)
	{
	case None: return "None";
	case True: return "True";
	case False: return "False";
	case NotEquiv: return "NotEquiv";
	case Equiv: return "Equiv";
	case Less: return "Less";
	case Greater: return "Greater";
	case LessOrEquiv: return "LessOrEquiv";
	case GreaterOrEquiv: return "GreaterOrEquiv";
	}
	return "UnknownTestType";
}

void Result::Write(StringStream &xml) const
{
	typedef XmlScopedElement::Property Prop;
	nstd::vector<Prop> props;
	props.push_back(Prop("type", type.ToString().c_str()));
	props.push_back(Prop("outcome", outcome.ToString().c_str()));
	props.push_back(Prop("file", location.file.c_str()));//.c_str()));//file.substr(location.file.find_last_of('\\') + 1).c_str()));
	StringStream line;
	line << location.line << Ends;
	props.push_back(Prop("line", line.ToString().c_str()));
	XmlScopedElement result(xml, "Result", props.begin(), props.end());
	if (!expression.Empty())
		xml << "<![CDATA[" << expression << "]]>";
}

String Summary::ToString() const
{
	StringStream S;
	S << *this;
	return S.ToString();
}

KAI_TEST_END

KAI_BEGIN

StringStream &operator<<(StringStream &S, const Test::Summary &C)
{
	S << "Module '" << C.module_name << "': " 
		<< C.num_passed << " passed, " 
		<< C.num_failed << " failed, " 
		<< C.num_not_implemented << " not implemented, " 
		<< C.num_exceptions << " exceptions";
	return S;
}

StringStream &operator<<(StringStream &S, const Test::XmlOutput &Q)
{
	return S << Q.xml.ToString();
}

KAI_END

#endif // KAI_UNIT_TESTS

//EOF
