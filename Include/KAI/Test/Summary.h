
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#if !defined(KAI_TEST_SUMMARY_H) && defined(KAI_UNIT_TESTS)
#	define KAI_TEST_SUMMARY_H

KAI_TEST_BEGIN

// Summary of execution of a set of tests
struct Summary
{
	typedef nstd::list<Result> Failures;

//protected:
	Failures failures;
	int num_passed, num_failed, num_not_implemented, num_exceptions;
	String start_time, finish_time;
	String module_name;

public:
	Summary()
	{
		num_passed = num_failed = num_not_implemented = num_exceptions = 0;
	}

	String ToString() const;

	static void Register(Registry &);
};

KAI_TEST_END

KAI_BEGIN

StringStream &operator<<(StringStream &, const Test::Summary &);

KAI_TYPE_TRAITS(Test::Summary, Number::TestSummary, Properties::StringStreamInsert);

KAI_END

#endif // KAI_TEST_SUMMARY_H

//EOF
