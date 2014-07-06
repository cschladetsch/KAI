

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#if defined(KAI_UNIT_TESTS) && !defined(KAI_TEST_SUITE_BASE_H)
#	define KAI_TEST_SUITE_BASE_H

KAI_TEST_BEGIN

// Common functionality for all Suite<>'s
class SuiteBase : public Component
{
protected:
	IOutput *output;

public:
	// Called before each test method
	void Pre();

	// Called after each test method
	void Post();

	void TestTrue(const char *file, int line, const char *exp, bool val)
	{
		AddResult(Result(FileLocation(file, line), Operation::True, val ? Outcome::Passed : Outcome::Failed, exp));
	}

	void TestNotTrue(const char *file, int line, const char *exp, bool val)
	{
		AddResult(Result(FileLocation(file, line), Operation::False, !val ? Outcome::Passed : Outcome::Failed, exp));
	}

	template <class First, class Second>
	void TestEquiv(const char *file, int line, const char *e1, const char *e2, First const &first, Second const &second)
	{
		bool result = first == second;
		StringStream exp;
		exp << e1 << " == " << e2 << Ends;
		AddResult(Result(FileLocation(file, line), Operation::Equiv, result ? Outcome::Passed : Outcome::Failed, exp.ToString()));
	}

	template <class First, class Second>
	void TestNotEquiv(const char *file, int line, const char *e1, const char *e2, First const &first, Second const &second)
	{
		bool result = first != second;
		StringStream exp;
		exp << e1 << " != " << e2 << Ends;
		AddResult(Result(FileLocation(file, line), Operation::NotEquiv, result ? Outcome::Passed : Outcome::Failed, exp.ToString()));
	}

	template <class First, class Second>
	void TestLess(const char *file, int line, const char *e1, const char *e2, First const &first, Second const &second)
	{
		bool result = first < second;
		StringStream exp;
		exp << e1 << " < " << e2 << Ends;
		AddResult(Result(FileLocation(file, line), Operation::Less, result ? Outcome::Passed : Outcome::Failed, exp.ToString()));
	}

	template <class First, class Second>
	void TestGreater(const char *file, int line, const char *e1, const char *e2, First const &first, Second const &second)
	{
		bool result = first > second;
		StringStream exp;
		exp << e1 << " > " << e2 << Ends;
		AddResult(Result(FileLocation(file, line), Operation::Greater, result ? Outcome::Passed : Outcome::Failed, exp.ToString()));
	}

	template <class First, class Second>
	void TestLessOrEquiv(const char *file, int line, const char *e1, const char *e2, First const &first, Second const &second)
	{
		bool result = first <= second;
		StringStream exp;
		exp << e1 << " <= " << e2 << Ends;
		AddResult(Result(FileLocation(file, line), Operation::LessOrEquiv, result ? Outcome::Passed : Outcome::Failed, exp.ToString()));
	}

	void TestNotImplemented(const char *file, int line, const char *reason = "")
	{
		AddResult(Result(FileLocation(file, line), Operation::None, Outcome::NotImplemented, reason));
	}

	void AddResult(Result const &result)
	{
		output->NewResult(result);
	}
};

KAI_TEST_END

#endif // KAI_TEST_SUITE_BASE_H

//EOF
