#include <KAI/Core/Config/Base.h>
#include <KAI/Core/Object.h>
#include <KAI/Core/PropertyBase.h>
#include <KAI/Core/ClassBase.h>

KAI_BEGIN

void SetPropertyValue(Object const &owner, Label const &name, Object value)
{
	if (!owner.Valid())
	{
		KAI_THROW_0(NullObject);
	}
	ClassBase const *klass = owner.GetClass();
	PropertyBase const &prop = klass->GetProperty(name);
	prop.SetValue(owner, value);
}

Object GetPropertyValue(Object const &owner, Label const &name)
{
	if (!owner.Exists())
		return Object();
	ClassBase const *klass = owner.GetClass();
	PropertyBase const &prop = klass->GetProperty(name);
	return prop.GetValue(owner);
}

void SetPropertyObject(Object const &owner, Label const &name, Object value)
{
	if (!owner.Exists())
		return;
	ClassBase const *klass = owner.GetClass();
	PropertyBase const &prop = klass->GetProperty(name);
	prop.SetObject(owner, value);
}

Object GetPropertyObject(Object const &owner, Label const &name)
{
	if (!owner.Exists())
		return Object();
	ClassBase const *klass = owner.GetClass();
	PropertyBase const &prop = klass->GetProperty(name);
	return prop.GetObject(owner);
}

KAI_END

