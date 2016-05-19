#pragma once

#include <KAI/Core/Config/Base.h>
#include <KAI/Core/Type/Number.h>
#include "Label.h"

KAI_BEGIN

namespace CreateParams 
{
	enum Params 
	{
		Create = 1,

		//< when supplied to a ClassBuilder<>'s property list, this indicates that the associated property will
		//< not be created when the owning object instance is created. for example:
		/*
		struct Foo
		{
			Pointer<Bar> bar;
		};
		ClassBuilder<Foo>("Foo", R)
			.Methods
			.Properties
				("bar", &Foo::bar, "a bar")
			;
		// in this case, when a new Foo object is created via Pointer<Foo> foo = New<Foo>(), the
		// encapsulated bar object will also be created via New<Bar>.
		//
		// however, if the `bar` property was added using the line:
		//		("bar", &Foo::bar, "a bar", Create::NoDefault)
		//									^^^^^^^^^^^^^^^^^
		// then the `bar` object in a new foo will be Null.
		*/
		NoCreate = 2,

		// default to no creation
		Default = NoCreate,
	};
}

/// nominates a field in an instance of a class. commonality for all accessors and mutators.
class PropertyBase
{
	Label field_name;
	Type::Number class_type;
	Type::Number field_type;
	bool is_system;
	int create_params;	// bitfield created from CreateParams::Params enumeration

public:
	PropertyBase(Label const &F, Type::Number C, Type::Number N, bool B, CreateParams::Params CP) : field_name(F), class_type(C), field_type(N), is_system(B), create_params(CP) { }
	virtual ~PropertyBase() { }

	String Description;

	/// true if field is an Object or Pointer<T>
	bool IsSystemType() const { return is_system; }

	/// if true, this property should be created when its containing object is created
	bool CreateDefaultValue() const { return (create_params & CreateParams::Create) == CreateParams::Create; }

	virtual void SetMarked(Object &, bool) = 0;

	virtual Object GetValue(Object const &) const = 0;
	virtual void SetValue(Object const &, Object const &) const = 0;

	virtual void SetObject(Object const &Q, Object const &V) const = 0;
	virtual Object GetObject(Object const &Q) const = 0;

	Label const &GetFieldName() const { return field_name; }
	Type::Number GetClassTypeNumber() const { return class_type; }
	Type::Number GetFieldTypeNumber() const { return field_type; }
};

StringStream &operator<<(StringStream &S, BasePointer<PropertyBase> const &);

KAI_TYPE_TRAITS(BasePointer<PropertyBase>,
	Number::Property, 
	Properties::StringStreamInsert);

// access the property object
void SetPropertyObject(Object const &owner, Label const &name, Object value);
Object GetPropertyObject(Object const &owner, Label const &name);

// access the property value
void SetPropertyValue(Object const &owner, Label const &name, Object value);
Object GetPropertyValue(Object const &owner, Label const &name);

template <class T>
void SetProperty(Object const &owner, Label const &name, T const &value) 
{ 
	SetPropertyValue(owner, name, owner.New(value)); 
}

template <class T>
T GetProperty(Object const &owner, Label const &name) 
{ 
	Object Q = GetPropertyValue(owner, name); 
	return ConstDeref<T>(Q);
}

KAI_END
