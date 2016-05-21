#pragma once

#include <KAI/Core/Base.h>

KAI_BEGIN

class StorageBase;
template <class T> class Storage;

class ClassBase;
template <class T> class Class;

class Object;
class String;
class StringStream;
class BinaryStream;
class BinaryPacket;
class Registry;
class Reflected;

template <class T>
struct FwdBasePointer { };

KAI_END

#include "KAI/Core/Object/Reflected.h"
