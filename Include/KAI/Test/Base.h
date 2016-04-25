#pragma once

#ifdef KAI_UNIT_TESTS

#include "KAI/KAI.h"
#include "KAI/Factory.h"
#include "KAI/BuiltinTypes/Signed32.h"
#include "KAI/BuiltinTypes/Bool.h"
#include "KAI/BuiltinTypes/Array.h"
#include "KAI/Operation.h"
#include "KAI/Continuation.h"
#include "KAI/Value.h"
#include "KAI/Reflected.h"
#include "KAI/Executor.h"
#include "KAI/Compiler.h"
#include "KAI/Pathname.h"
#include "KAI/XmlScopedElement.h"

#define KAI_TEST_BEGIN KAI_BEGIN namespace Tests {
#define KAI_TEST_END KAI_END }

KAI_TEST_BEGIN

class Component;
class SuiteBase;
class Module;

// The nominal result of a test
struct Outcome
{
	enum Type { Passed = 1, Failed = 2, None = 3, Exception = 4, NotImplemented = 5};
	Type type;
	Outcome(Type V = None) : type(V) { }
	friend bool operator==(Outcome A, Outcome B) { return A.type == B.type; }
	friend bool operator!=(Outcome A, Outcome B) { return A.type != B.type; }
	String ToString() const;
};

// The relationship to test
struct Operation
{
	enum Type { None = 1, True = 2, False = 3 , Equiv = 4, NotEquiv = 5, Less = 6, Greater = 7, LessOrEquiv = 8, GreaterOrEquiv = 9};
	Type type;
	Operation(Type V = None) : type(V) { }
	friend bool operator==(Operation A, Operation B) { return A.type == B.type; }
	friend bool operator!=(Operation A, Operation B) { return A.type != B.type; }
	String ToString() const;
};

// A specific test result
struct Result
{
	String expression;
	FileLocation location;
	Operation type;
	Outcome outcome;

public:
	Result() { }
	Result(FileLocation const &L1, Operation T1, Outcome O1, const String &E1) : expression(E1), location(L1), type(T1), outcome(O1) { }

	void Write(StringStream& xml) const;
};

#define KAI_TESTS_TYPE_TRAITS(T,N,P) \
	KAI_TESTS_END \
	KAI_BEGIN \
	KAI_TYPE_TRAITS(Tests::T,N,P); \
	KAI_END \
	KAI_TESTS_BEGIN

KAI_TEST_END

#include "KAI/Test/Summary.h"
#include "KAI/Test/IOutput.h"
#include "KAI/Test/Component.h"
#include "KAI/Test/SuiteBase.h"
#include "KAI/Test/Module.h"
#include "KAI/Test/Suite.h"
#include "KAI/Test/BasicOutput.h"
#include "KAI/Test/XmlOutput.h"
#include "KAI/Test/Macros.h"

#endif // KAI_UNIT_TESTS

//EOF
