#include <map>

#include <KAI/Core/Object/IObject.h>
#include <KAI/Core/Registry.h>

KAI_BEGIN

void StorageBase::Detach(Object const &Q)
{
    Dictionary::const_iterator A = dictionary.begin(), B = dictionary.end();
    for (; A != B; ++A)
    {
        if (A->second.GetHandle() == Q.GetHandle())
        {
            Detach(A->first);
            return;
        }
    }
}

void StorageBase::SetSwitch(int S, bool N)
{
    if (N)
        switches |= S;
    else
        switches &= ~S;

    const ClassBase *klass = GetClass();
    if (klass != 0 && S != Clean)
        klass->SetSwitch(*this, S, N);
}

void StorageBase::SetParentHandle(Handle H)
{
    parent = H;
}

Object StorageBase::Get(const Label &L) const
{
    if (GetClass()->HasProperty(L))
        return GetClass()->GetProperty(L).GetValue(*this);

    Dictionary::const_iterator A = dictionary.find(L);
    if (A == dictionary.end())
        return Object();

    return A->second;
}

void StorageBase::Set(const Label &name, Object const &child)
{
    if (child.GetHandle() == GetHandle())
        KAI_THROW_1(InternalError, "Recursion");

    // mark the object as being altered
    SetDirty();
    
    // set a property if it exists
    ClassBase const *klass = GetClass();
    if (klass->HasProperty(name))
    {
        klass->GetProperty(name).SetValue(*this, child);
        return;
    }

    // otherwise this is a child object. remove any existing child
    Remove(name);

    // update the child object
    if (!child.Exists())
    {
        Dictionary::iterator ch = dictionary.find(name);
        if (ch != dictionary.end())
            dictionary.erase(ch);
        return;
    }

    StorageBase &base = KAI_NAMESPACE(GetStorageBase(child));
    base.SetLabel(name);
    base.SetParentHandle(GetHandle());
    
    bool clean = base.IsClean();
    bool konst = base.IsConst();
    bool managed = base.IsManaged();
    base.switches = switches;                // inherit properties of parent...

    if (clean)                                // ...but preserve cleanliness
        base.switches |= IObject::Clean;
    else
        base.switches &= ~IObject::Clean;

    if (konst)                                // ...and constness
        base.switches |= IObject::Const;

    if (managed)                            // ...and managed
        base.switches |= IObject::Managed;

    // add it to this dictionary, inform it of being added to a container
    dictionary[name] = child;
    base.AddedToContainer(*this);
}

bool StorageBase::Has(const Label &L) const
{
    Dictionary::const_iterator object = dictionary.find(L);
    return object != dictionary.end() && object->second.Exists();
}

void StorageBase::Remove(const Label &label)
{
    Dictionary::iterator iter = dictionary.find(label);
    if (iter == dictionary.end())
        return;

    SetDirty();
    StorageBase *child = iter->second.GetBasePtr();
    dictionary.erase(iter);

    if (child)
    {
        child->SetParentHandle(Handle());
        child->RemovedFromContainer(*this);
    }
}

void StorageBase::SetColorRecursive(ObjectColor::Color color)
{
    HandleSet handles;
    SetColorRecursive(color, handles);
}

// avoid loops by passing history of objects traversed via handles argument
void StorageBase::SetColorRecursive(ObjectColor::Color color, HandleSet& handles)
{
    Handle handle = GetHandle();
    if (handles.find(handle) != handles.end())
        return;

    handles.insert(handle);

    if (!SetColor(color))
        return;

    GetClass()->SetReferencedObjectsColor(*this, color, handles);
    if (dictionary.empty())
        return;

    for (Dictionary::value_type const &child : dictionary)
    {
        StorageBase *sub = GetRegistry()->GetStorageBase(child.second.GetHandle());
        if (!sub)
            continue;

        sub->SetColorRecursive(color, handles);
    }
}

bool StorageBase::SetColor(ObjectColor::Color color)
{
    auto reg = GetRegistry();
    if (!reg->SetColor(*this, color))
        return false;

    this->color = color;
    if (color == ObjectColor::White)
    {
        Containers::const_iterator container = containers.begin(), end = containers.end();
        for (; container != end; ++container)
        {
            StorageBase *cont = GetRegistry()->GetStorageBase(*container);
            if (cont && cont->IsBlack())
                cont->SetColor(ObjectColor::Grey);
        }
    }

    return true;
}

void StorageBase::MakeReachableGrey()
{
    Dictionary::const_iterator child = dictionary.begin(), end = dictionary.end();
    for (; child != end; ++child)
    {
        StorageBase *sub = GetRegistry()->GetStorageBase(child->second.GetHandle());
        if (!sub)
            continue;

        if (sub->IsWhite())
            sub->SetColor(ObjectColor::Grey);
    }

    GetClass()->MakeReachableGrey(*this);
}

bool StorageBase::CanBlacken()
{
    KAI_NOT_IMPLEMENTED();
}

void StorageBase::RemovedFromContainer(Object const &container)
{
    ObjectColor::Color color = ObjectColor::White;
    StorageBase *parent = GetRegistry()->GetStorageBase(GetParentHandle());
    bool parent_is_black = parent && parent->IsBlack();
    if (parent_is_black)
        color = ObjectColor::Grey;
    
    bool removed = false;
    auto iter = containers.begin(), end = containers.end();
    for (; iter != end; )
    {
        StorageBase *base = GetRegistry()->GetStorageBase(*iter);
        if (!base)
        {
            iter = containers.erase(iter);
            continue;
        }

        if (!removed && *iter == container.GetHandle())
        {
            iter = containers.erase(iter);
            removed = true;
            if (parent_is_black)
            {
                // if removed from container and parent is black early out
                break;
            }
            else
            {
                // we need to check for other black parents to enforce the TriColor invariant
                continue;
            }
        }

        if (base->IsBlack())
        {
            color = ObjectColor::Grey;
            parent_is_black = true;
            // if any parent container is black, and we have already removed from the
            // given container, we can early out
            if (removed)
                break;
        }

        ++iter;
    }

    SetColorRecursive(color);
}

void StorageBase::DetermineNewColor() 
{ 
    // removing from an empty container will still traverse through other containers to determine new color
    RemovedFromContainer(Object()); 
}


void StorageBase::AddedToContainer(Object const &container)
{
    if (container.GetHandle() == GetHandle())
        KAI_THROW_1(InternalError, "Can't add a container to itself");

    containers.push_back(container.GetHandle());
    if (IsWhite())
        SetGrey();
}

void StorageBase::SetClean(bool clean)
{
    SetSwitch(Clean, clean);
    if (!clean && IsBlack())
        SetColor(ObjectColor::Grey);
}

void StorageBase::DetachFromContainers()
{
    if (containers.empty())
        return;

    Containers tmp = containers;
    Containers::const_iterator iter = tmp.begin(), end = tmp.end();
    for (; iter != end; ++iter)
    {
        StorageBase *cont = GetRegistry()->GetStorageBase(*iter);
        if (!cont)
            continue;

        cont->GetClass()->DetachFromContainer(*cont, *this);
    }
}

void StorageBase::Delete()
{
    // avoid double deletion
    if (IsMarked())
        return;

    SetManaged(true);

    // remove from all containers
    DetachFromContainers();

    // remove from parent
    StorageBase *parent = GetParentBasePtr();
    if (parent != 0)
        parent->Remove(GetLabel());

    // set this and all referent objects to be white, and mark it for deletion.
    SetColorRecursive(ObjectColor::White);
    SetMarked(true);
}

void StorageBase::SetManaged(bool managed) 
{ 
    if (!managed)
        SetColor(ObjectColor::Black);

    SetSwitch(Managed, managed); 
}

KAI_END

//EOF
