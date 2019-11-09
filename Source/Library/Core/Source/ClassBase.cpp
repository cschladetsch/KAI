#include <KAI/Core/Object/Class.h>

KAI_BEGIN

void RegisterClass(Registry &reg, ClassBase const &klass, Object const &root, Pathname const &path)
{
    // TODO: what were they ever for?
    KAI_UNUSED_2(root, path);
    reg.AddClass(&klass);
}

ClassBase::~ClassBase()
{
    for (const auto& method : methods)
        delete method.second;

    for (auto const &prop : properties)
        delete prop.second;
}

void ClassBase::SetReferencedObjectsColor(
        StorageBase &base, ObjectColor::Color color, HandleSet& handles) const
{
    if (properties.empty())
        return;

    for (auto const &iter : properties)
    {
        PropertyBase const &prop = *(iter.second);
        if (!prop.IsSystemType())
            continue;

        Object property = prop.GetObject(base);
        if (!property.Exists())
            continue;

        property.SetColorRecursive(color, handles);
    }
}

void ClassBase::SetMarked(StorageBase &Q, bool M) const
{
    Properties::const_iterator A = properties.begin(), B = properties.end();
    for (; A != B; ++A)
        A->second->SetMarked(Q, M);
    
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

