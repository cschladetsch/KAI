
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_METHOD_BASE_H
#	define KAI_METHOD_BASE_H

KAI_BEGIN

class MethodBase : public CallableBase<MethodBase>
{
protected:
	Type::Number class_type;
	Constness constness;

public:
	MethodBase(Constness C, const Label &N) : constness(C), CallableBase<MethodBase>(N) { }

	Type::Number GetClassType() const { return class_type; }
	Constness GetConstness() const { return constness; }

	void Invoke(Object const &Q, Stack &stack)
	{
		if (Q.IsConst())
			ConstInvoke(Q, stack);
		else
			NonConstInvoke(Q, stack);
	}

	virtual void NonConstInvoke(const Object &servant, Stack &stack) = 0;
	virtual void ConstInvoke(const Object &servant, Stack &stack) = 0;

	Object GetArgumentTypesArray() const;
	String ToString() const;
	static void Register(Registry &);
};

StringStream &operator<<(StringStream &, const BasePointer<MethodBase> &);

KAI_TYPE_TRAITS(BasePointer<MethodBase>, Number::Method, Properties::StringStreamInsert | Properties::Reflected);

KAI_END

#endif // KAI_METHOD_BASE_H

//EOF
