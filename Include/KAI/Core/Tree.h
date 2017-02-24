#pragma once

#include <KAI/Core/Config/Base.h>
#include <KAI/Core/Exception/ExceptionMacros.h>
#include <KAI/Core/Pathname.h>
#include <KAI/Core/Object/Object.h>

KAI_BEGIN

class Tree
{
public:
	typedef std::list<Object> SearchPath;

private:
	SearchPath path;
	Object root, scope;
	Pathname current;

public:
	void SetRoot(const Object &Q) { root = Q; }
	void SetSearchPath(const SearchPath &);
	void AddSearchPath(const Pathname &P);
	void AddSearchPath(const Object &P);

	Object Resolve(const Pathname &) const;
	Object Resolve(const Label &) const;

	Object GetRoot() const { return root; }
	Object GetScope() const { return scope; }
	const SearchPath &GetSearchPath() const { return path; }

	void SetScope(const Object &);
	void SetScope(const Pathname &);
};

KAI_END
