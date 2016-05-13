#pragma once

KAI_BEGIN

// TODO: this does not need to be a template
template <class Callable>
struct CallableBase : BasePointerBase
{
	typedef std::vector<Type::Number> ArgumentTypes;

protected:
	Type::Number return_type;
	ArgumentTypes arguments;
	Label name;

public:
	CallableBase() { }
	CallableBase(const Label &N) : name(N) { }

	Type::Number GetReturnType() const { return return_type; }
	Label GetName() const { return name; }
	const ArgumentTypes &GetArgumentTypes() const { return arguments; }
	Type::Number GetArgumentType(int N) const { return arguments[N]; }
	String Description;

	Object GetArgumentTypesArray() const;
	/*
	{
		Pointer<Array> args = New<Array>();
		ArgumentTypes::const_iterator A = arguments.begin(), B = arguments.end();
		for (; A != B; ++A)
			args->Append(New(*A));
		return args;
	}
	*/
};

namespace Type
{
	template <class T>
	struct Traits<CallableBase<T> > : 
	TraitsBase<CallableBase<T> *, Type::Number::Callable, 0> { };
};

KAI_END
