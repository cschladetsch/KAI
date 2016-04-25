
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#if defined(KAI_UNIT_TESTS) && !defined(KAI_TEST_SUITE_H)
#	define KAI_TEST_SUITE_H

KAI_TEST_BEGIN

/// A collection of test methods pertaining to one target class under test
template <class Class>
class Suite : public SuiteBase
{
	struct Method
	{
		typedef void (Class::*MethodPointer)(); 
		MethodPointer method;
		String name;

		Method() { }
		Method(String const &N, MethodPointer P) : name(N), method(P) { }
	};
	typedef std::list<Method> Methods;
	Methods methods;

public:
	void Add(Method const &method)
	{
		methods.push_back(method);
	}

	void Run(IOutput *output)
	{
		this->output = output;
		Class *instance = reinterpret_cast<Class *>(this);
		Methods::const_iterator A = methods.begin(), B = methods.end();
		for (; A != B; ++A)
		{
			output->MethodBegin(A->name);
			try
			{
				Pre();
				(instance->*A->method)();
				Post();
			}
			catch (const Exception::Base &E)
			{
				output->NewException(E);
			}
			catch (const std::exception &E)
			{
				output->NewStdException(E);
			}
			catch (...)
			{
				output->NewUnknownException();
			}
			output->MethodEnd(A->name);
		}
	}

	/// Utility structure to assist with naming and adding test methods
	struct Builder
	{
		Suite<Class> *suite;
		Builder(Suite<Class> *S, const char *name)
		{
			suite = S;
			suite->Name = name;
		}
		Builder &operator()(const char *name, typename Method::MethodPointer method)
		{
			suite->Add(Method(String(name), method));
			return *this;
		}
	};
};

KAI_TEST_END

#endif // defined(KAI_UNIT_TESTS) && !defined(KAI_TEST_SUITE_H)

//EOF
