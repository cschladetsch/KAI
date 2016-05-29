#include <KAI/Core/Object/Class.h>

KAI_BEGIN

void RegisterClass(Registry &reg, ClassBase const &klass, Object const &root, Pathname const &path)
{
    // TODO: what were there ever for?
	KAI_UNUSED_2(root, path);
	reg.AddClass(&klass);
}

ClassBase::~ClassBase()
{
	Methods::iterator A = methods.begin(), B = methods.end();
	for (; A != B; ++A)
		delete A->second;
	Properties::iterator C = properties.begin(), D = properties.end();
	for (; C != D; ++C)
		delete C->second;
}

void ClassBase::SetMarked(StorageBase &Q, bool M) const
{
	Properties::const_iterator A = properties.begin(), B = properties.end();
	for (; A != B; ++A)
	{
		A->second->SetMarked(Q, M);
	}
	SetMarked2(Q, M);
}

void ClassBase::MakeReachableGrey(StorageBase &base) const
{
	if (properties.empty())
		return;
	ClassBase::Properties::const_iterator iter = properties.begin(), end = properties.end();
	for (; iter != end; ++iter)
	{
		PropertyBase const &prop = *iter->second;
		if (!prop.IsSystemType())
			continue;
		Object property = prop.GetObject(base);
		StorageBase *b = property.GetRegistry()->GetStorageBase(property.GetHandle());
		if (b == 0)
			continue;
		if (b->IsWhite())
			b->SetColor(ObjectColor::Grey);
	}
}

void ClassBase::GetPropertyObjects(StorageBase &object, ObjectList &contained) const
{
	if (properties.empty())
		return;
	ClassBase::Properties::const_iterator iter = properties.begin(), end = properties.end();
	for (; iter != end; ++iter)
	{
		PropertyBase const &prop = *iter->second;
		if (!prop.IsSystemType())
			continue;
		Object property = prop.GetObject(object);
		StorageBase *base = property.GetRegistry()->GetStorageBase(property.GetHandle());
		if (base == 0)
			continue;
		contained.push_back(*base);
	}
}

KAI_END

