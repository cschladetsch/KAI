#pragma once

#include <KAI/Core/Config/Base.h>
#include <KAI/Core/Object/Object.h>
#include <KAI/Core/Pathname.h>

KAI_BEGIN

class Tree {
   public:
    typedef std::list<Object> SearchPath;

   private:
    SearchPath _path;
    Object _root, _scope;
    Pathname _current;

   public:
    void SetRoot(const Object &Q) { _root = Q; }
    void AddSearchPath(const Pathname &);
    void AddSearchPath(const Object &);

    Object Resolve(const Pathname &) const;
    Object Resolve(const Label &) const;

    Object GetRoot() const { return _root; }
    Object GetScope() const { return _scope; }
    const SearchPath &GetSearchPath() const { return _path; }

    void SetScope(const Object &);
    void SetScope(const Pathname &);

    // void SetSearchPath(const SearchPath &);
    // void GetChildren() const;
};

KAI_END
