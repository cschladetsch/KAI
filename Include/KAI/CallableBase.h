
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_CALLABLE_BASE_H
#	define KAI_CALLABLE_BASE_H

KAI_BEGIN

// TODO: this does not need to be a template
template <class Callable>
struct CallableBase : BasePointerBase
{
	typedef std::vector<Type::NumberEnum> ArgumentTypes;

protected:
	Type::NumberEnum return_type;
	ArgumentTypes arguments;
	Label name;

public:
	CallableBase() { }
	CallableBase(const Label &N) : name(N) { }

	Type::NumberEnum GetReturnType() const { return return_type; }
	Label GetName() const { return name; }
	const ArgumentTypes &GetArgumentTypes() const { return arguments; }
	Type::NumberEnum GetArgumentType(int N) const { return arguments[N]; }
	String Description;

	Object GetArgumentTypesArray() const
	{
		Pointer<Array> args = New<Array>();
		ArgumentTypes::const_iterator A = arguments.begin(), B = arguments.end();
		for (; A != B; ++A)
			args->Append(New(*A));
		return args;
	}
};

namespace Type
{
	template <class T>
	struct Traits<CallableBase<T> > : TraitsBase<CallableBase<T> *, Type::Number::Callable, 0> { };
};

KAI_END

#endif // KAI_CALLABLE_BASE_H

//EOF
