

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_TREE_H
#	define KAI_TREE_H

KAI_BEGIN

struct Tree
{
	typedef nstd::list<Object> SearchPath;

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

//#include KAI_TEST(X)

#endif // KAI_TREE_H

//EOF

