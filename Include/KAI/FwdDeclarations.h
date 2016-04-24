
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_FWD_DECLARATIONS_H
#	define KAI_FWD_DECLARATIONS_H

KAI_BEGIN

struct StorageBase;
class ClassBase;
struct Object;

template <class T> struct Storage;

struct String;

struct StringStream;
struct BinaryStream;
struct BinaryPacket;
struct Stack;
struct Registry;

struct Handle;

typedef boost::unordered_set<Handle> StorageBaseHandles;

KAI_END

#endif // KAI_FWD_DECLARATIONS_H

//EOF
