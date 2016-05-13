#pragma once

KAI_BEGIN

struct FunctionBase : CallableBase<FunctionBase>
{
	FunctionBase(const Label &L) : CallableBase<FunctionBase>(L) { }

	virtual void Invoke(Registry &, Stack &stack) = 0;
	String ToString() const;
	static void Register(Registry &);
};

StringStream &operator<<(StringStream &, FunctionBase const &);
//StringStream &operator<<(StringStream &, const BasePointer<FunctionBase> &);

KAI_TYPE_TRAITS(BasePointer<FunctionBase>, Number::Function
	, Properties::StringStreamInsert 
	);//| Properties::Reflected);

KAI_END

