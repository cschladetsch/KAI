
#include "KAI/KAI.h"
#include "KAI/Pathname.h"
#include "KAI/Tree.h"
#include "KAI/BuiltinTypes/Void.h"

#include <iostream>

KAI_BEGIN

// ICE ICE BABY TOO COLD
//template <class, bool> struct C { };
//struct D { };
//template <class A> struct C<A, false> : C<A, false>, D { };

Pathname GetFullname(const Object &Q)
{
	if (!Q.Valid())
		return Pathname();
	return GetFullname(GetStorageBase(Q));
}

Pathname GetFullname(const StorageBase &Q)
{
	nstd::list<String> parentage;
	Label const &label = Q.GetLabel();
	if (!label.ToString().empty())
		parentage.push_back(label.ToStringUnquoted());
	Object parent = Q.GetParent();
	for (; parent.Valid(); parent = parent.GetParent())
	{
		parentage.push_front(GetStorageBase(parent).GetLabel().ToStringUnquoted());
	}
	StringStream path;
	if (!parentage.empty())
		;//path << "[anon]";
	else
		path.Append('/');
	foreach (String const &name, parentage)
	{
		path.Append(name);
		path.Append('/');
	}
	return path.ToString();
}

void Set(Object scope, const Pathname &path, Object const &Q)
{
	if (path.Empty())
		return;
	Pathname::Elements::const_iterator A = path.begin(), B = --path.end();
	if (B->type != Pathname::Element::Name)
		KAI_THROW_1(InvalidPathname, path.ToString());
	for (; A != B; ++A)
	{
		switch (A->type)
		{
		case Pathname::Element::Seperator: break;
		case Pathname::Element::This: break;
		case Pathname::Element::Parent: scope = scope.GetParent(); break;
		case Pathname::Element::Name: 
			{
				const Label &name = A->name;
				if (A == --path.end())
				{
					GetStorageBase(scope).Set(name, Q);
				}
				else
				{
					StorageBase &base = GetStorageBase(scope);
					if (!base.Has(name))
					{
						Object added = scope.New<void>();
						base.Set(name, added);
						scope = added;
					}
					else
					{
						scope = base.Get(name);
					}
				}
			}
			break;
		}
	}
	GetStorageBase(scope).Set(A->name, Q);
}

void Set(Object const &root, Object const &scope, Object const &ident, Object const &Q)
{
	switch (GetTypeNumber(ident).value)
	{
	case Type::Number::Label:
		{
			const Label &label = ConstDeref<Label>(ident);
			GetStorageBase(scope).Set(label, Q);
			break;
		}
	case Type::Number::Pathname:
		{
			const Pathname &path = ConstDeref<Pathname>(ident);
			if (path.Absolute)
				Set(root, path, Q);
			else
				Set(scope, path, Q);
			break;
		}
	default:
		KAI_THROW_1(InvalidIdentifier, Q);
	}
}

Object Get(Object const &root, Object const &scope, const Pathname &path)
{
	if (path.Empty())
		return path.Absolute ? root : Object();
	if (path.Absolute)
		return Get(root, path);
	return Get(scope, path);
}

Object Get(Object scope, const Pathname &path)
{
	if (!scope.Exists())
		return Object();

	const Pathname::Elements &elements = path.GetElements();
	Pathname::Elements::const_iterator A = elements.begin(), B = elements.end();

	for (; A != B; ++A)
	{
		switch (A->type)
		{
		case Pathname::Element::Seperator: break;
		case Pathname::Element::Name: 
			{
				scope = scope.Get(A->name);
				if (!scope)
					return scope;
			}
			break;
		case Pathname::Element::This: break;
		case Pathname::Element::Parent: scope = scope.GetParent(); break;
		}
	}
	return scope;
}

bool Exists(Object const &scope, const Pathname &path)
{
	try
	{
		return Get(scope, path).Exists();
	}
	catch (Exception::ObjectNotFound &)
	{
	}
	return false;
}

bool Exists(Object const &root, Object const &scope, const Pathname &path)
{
	try
	{
		return Get(root, scope, path).Exists();
	}
	catch (Exception::ObjectNotFound &)
	{
	}
	return false;
}

void Remove(Object scope, const Pathname &path)
{
	if (path.Empty())
		KAI_THROW_0(Base);
	const Pathname::Elements &elements = path.GetElements();
	if (elements.back().type != Pathname::Element::Name)
		KAI_THROW_0(Base);

	Pathname::Elements::const_iterator A = elements.begin(), B = elements.end();
	for (--B; A != B; ++A)
	{
		switch (A->type)
		{
		case Pathname::Element::Seperator: break;
		case Pathname::Element::Name: 
			scope = scope.Get(A->name);
			break;
		case Pathname::Element::This: break;
		case Pathname::Element::Parent: 
			scope = scope.GetParent();
			break;
		}
	}
	scope.Remove(A->name);
}

void Remove(Object const &root, Object const &scope, const Pathname &path)
{
	if (path.Absolute)
		Remove(root, path);
	else
		Remove(scope, path);
}

void Remove(Object const &root, Object const &scope, Object const &ident)
{
	const StorageBase &id = ident.GetStorageBase();
	if (id.GetTypeNumber() == Type::Traits<Label>::Number)
	{
		scope.Remove(ConstDeref<Label>(id));
		return;
	}
	Remove(root, scope, ConstDeref<Pathname>(id));
}

Label GetName(const Object &object)
{
	if (!object)
		return Label();
	return object.GetLabel();
}

//---------------------------------------------------------------
void Tree::SetScope(const Object &Q)
{
	Pathname path = GetFullname(Q);
	if (!Exists(root, scope, path))
		KAI_THROW_1(Base, "ObjectNotInTree");
	scope = Q;
}

void Tree::SetScope(const Pathname &path)
{
	if (!Exists(root, scope, path))
		KAI_THROW_0(ObjectNotFound);
	scope = Get(root, scope, path);
}

#undef SearchPath

void Tree::SetSearchPath(const SearchPath &P)
{
	path = P;
}

void Tree::AddSearchPath(const Object &P)
{
	path.push_back(P);
}

void Tree::AddSearchPath(const Pathname &P)
{
	path.push_back(Get(root, scope, P));
}

Object Tree::Resolve(const Label &label) const
{
	if (scope)
	{
		Object found = scope.Get(label);
		if (found)
			return found;
	}
	foreach (Object const &object, path)
	{
		if (!object)
			continue;
		Object found = object.Get(label);
		if (found)
			return found;
	}
	return Object();
}

Object Tree::Resolve(const Pathname &P) const
{
	Object found = Get(root, scope, P);
	if (found.Exists())
		return found;
	if (P.Absolute)
		return Object();
	SearchPath::const_iterator A = path.begin(), B = path.end();
	for (; A != B; ++A)
	{
		Object found = Get(root, *A, P);
		if (found.Exists())
			return found;
	}
	return Object();
}

KAI_END

//EOF
