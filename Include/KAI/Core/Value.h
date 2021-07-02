#pragma once

#include <KAI/Core/Config/Base.h>
#include <KAI/Core/Type/Number.h>
#include <KAI/Core/Type/Traits.h>
#include <KAI/Core/Object/GetStorageBase.h>

KAI_BEGIN

// A ConstValue<> has direct access to the storage of an object.
// This means that accessing the value is faster as there is no
// lookup required. However, it is unsafe to use a Value over multiple
// frames, as the storage could be deleted by the garbage collector.
// So it is only really safe to use a ConstValue<> in local scope,
// unless you can be sure that the corresponding Object will not
// be collected.
template <class T>
class ConstValue
{
protected:
    typedef Type::Traits<T> Traits;
    typedef typename Traits::Store Store;
    typedef typename Traits::Reference Reference;
    typedef typename Traits::ConstReference ConstReference;

    Storage<Store> *storage;

public:

    ConstValue() : storage(0) { }

    ConstValue(Object const &Q) : storage(0)
    {
        AssignFrom(Q);
    }
    
    operator Object() const
    {
        return *storage;
    }

    ConstValue<T> &operator=(const ConstValue<T> &Q)
    {
        AssignFrom(Q.GetObject());
        return *this;
    }

    ConstValue<T> &operator=(Object const &Q)
    {
        AssignFrom(Q);
        return *this;
    }

    void AssignFrom(Object const &Q)
    {
        storage = 0;
        if (!Q.Exists() || !Q.Valid())
            return;
        Type::Number type = Q.GetTypeNumber();
        if (type == Type::Number::None)
            return;
        if (type != Type::Traits<T>::Number)
            KAI_THROW_0(TypeMismatch);
        storage = &GetStorage<T>(Q);
    }

    ConstReference operator*() const
    { 
        if (storage == 0)
            KAI_THROW_0(NullObject);
        return storage->GetConstReference();
    }

    const T *operator->() const
    { 
        if (storage == 0)
            KAI_THROW_0(NullObject);
        return &storage->GetConstReference();
    }

    Handle GetHandle() const 
    { 
        return Exists() ? GetObject().GetHandle() : Handle();
    }

    Registry *GetRegistry() const
    {
        return Exists() ? GetObject().GetRegistry() : 0;
    }

    Type::Number GetTypeNumber() const
    {
        return Exists() ? GetObject().GetTypeNumber() : Type::Number::None;
    }
    
    void SetSwitch(int S, bool M)
    {
        if (storage)
            storage->SetSwitch(S, M);
    }
    void SetMarked(bool M)
    {
        storage->SetMarked(M);
    }
    bool Valid() const
    {
        return Exists() ? GetObject().Valid() : false;
    }

    bool Exists() const
    {
        return storage && GetObject().Exists();
    }

    bool IsManaged() const
    {
        return Exists() && storage->IsManaged();
    }

    bool IsConst() const
    {
        return Exists() && GetObject().IsConst();
    }

    bool IsMutable() const
    {
        return Exists() && GetObject().IsMutable();
    }

    Object &GetObject() const
    {
        return *storage;
    }

    const Object &GetConstObject() const
    {
        return *storage;
    }
};

// A Value<> has direct access to the storage of an object.
// This means that accessing the value is faster as there is no
// lookup required. However, it is unsafe to use a Value over multiple
// frames, as the storage could be deleted by the garbage collector.
// So it is only really safe to use a Value<> in local scope,
// unless you can be sure that the corresponding Object will not
// be collected.
template <class T>
struct Value : ConstValue<T>
{
    using typename ConstValue<T>::Store;
    using typename ConstValue<T>::Reference;
    using typename ConstValue<T>::ConstReference;

    using ConstValue<T>::storage;
    using ConstValue<T>::AssignFrom;

    Value() { }

    Value(Object const &Q)
    {
        AssignFrom(Q);
    }

    Value<T> &operator=(Value<T> const &Q)
    {
        ConstValue<T>::operator=(Q);
        return *this;
    }

    Value<T> &operator=(Object const &Q)
    {
        ConstValue<T>::operator=(Q);
        return *this;
    }

    Reference operator*()
    { 
        if (storage == 0)
            KAI_THROW_0(NullObject);
        return storage->GetReference();
    }

    T *operator->()
    { 
        if (storage == 0)
            KAI_THROW_0(NullObject);
        return &storage->GetReference();
    }

    ConstReference operator*() const
    { 
        return ConstValue<T>::operator*();
    }

    const T *operator->() const
    { 
        return ConstValue<T>::operator->();
    }
};

template <class T>
struct Value<const T> : ConstValue<T>
{
    using typename ConstValue<T>::Store;
    using typename ConstValue<T>::Reference;
    using typename ConstValue<T>::ConstReference;

    Value() { }

    Value(Object const &Q) : ConstValue<T>(Q) { }

    Value<T> &operator=(Object const &Q)
    {
        ConstValue<T>::operator=(Q);
        return *this;
    }

    ConstReference operator*() const
    { 
        return ConstValue<T>::operator*();
    }

    const T *operator->() const
    { 
        return ConstValue<T>::operator->();
    }
};

KAI_END
