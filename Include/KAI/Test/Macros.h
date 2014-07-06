
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#if defined(KAI_UNIT_TESTS) && !defined(KAI_TEST_MACROS_H)
#	define KAI_TEST_SUITE_BASE_H

// A set of macros to perform a test using a given
// test suite base object

#	define KAI_TEST_TRUE_GIVEN(This, X) \
	This->TestTrue(__FILE__, __LINE__, #X, X);

#	define KAI_TEST_ASSERT_GIVEN(This, X) \
	KAI_TEST_TRUE_GIVEN(This, X)

#	define KAI_TEST_NOT_TRUE_GIVEN(This, X) \
	This->TestNotTrue(__FILE__, __LINE__, #X, X)

#	define KAI_TEST_FALSE_GIVEN(This, X) \
	KAI_TEST_NOT_TRUE_GIVEN(This,X)

#	define KAI_TEST_UNTRUE_GIVEN(This, X) \
	KAI_TEST_NOT_TRUE_GIVEN(This,X)

#	define KAI_TEST_EQUIV_GIVEN(This, A, B) \
	This->TestEquiv(__FILE__, __LINE__, #A, #B, A, B)

#	define KAI_TEST_NOT_EQUIV_GIVEN(This, A, B) \
	This->TestNotEquiv(__FILE__, __LINE__, #A, #B, A, B)

#	define KAI_TEST_NOT_EQUAL_GIVEN(This, A, B) \
	This->TestNotEquiv(__FILE__, __LINE__, #A, #B, A, B)

#	define KAI_TEST_EQUAL_GIVEN(This, A, B) \
	KAI_TEST_EQUIV_GIVEN(This, A, B)

#	define KAI_TEST_LESS_GIVEN(This, A, B) \
	This->TestLess(__FILE__, __LINE__, #A, #B, A, B)

#	define KAI_TEST_LESS_OR_EQUIV_GIVEN(This, A, B) \
	This->TestLessOrEquiv(__FILE__, __LINE__, #A, #B, A, B)

#	define KAI_TEST_LESS_OR_EQUAL_GIVEN(This, A, B) \
	KAI_TEST_LESS_OR_EQUIV_GIVEN(This,A, B)

#	define KAI_TEST_NOT_GREATER_GIVEN(This, A, B) \
	KAI_TEST_LESS_OR_EQUAL_GIVEN(This,A, B)

#	define KAI_TEST_GREATER_GIVEN(This, A, B) \
	This->TestGreater(__FILE__, __LINE__, #A, #B, A, B)

#	define KAI_TEST_NOT_IMPLEMENTED_GIVEN(This) \
	This->TestNotImplemented(__FILE__, __LINE__)

#	define KAI_TEST_NOT_IMPLEMENTED_TEXT_GIVEN(This, T) \
	This->TestNotImplemented(__FILE__, __LINE__, T)

#	define KAI_TEST_DEPRECATED_GIVEN(This, T) \
	This->Deprecated(T)

#	define KAI_TEST_MESSAGE_GIVEN(This, T) \
	This->Message(T)

// As above, but defaultint to use `this` as the test suite

#	define KAI_TEST_TRUE(X) \
	KAI_TEST_TRUE_GIVEN(this, X)

#	define KAI_TEST_ASSERT(X) \
	KAI_TEST_TRUE_GIVEN(this, X)

#	define KAI_TEST_NOT_TRUE(X) \
	KAI_TEST_NOT_TRUE_GIVEN(this, X)

#	define KAI_TEST_FALSE(X) \
	KAI_TEST_FALSE_GIVEN(this, X)

#	define KAI_TEST_UNTRUE(X) \
	KAI_TEST_UNTRUE_GIVEN(this, X)

#	define KAI_TEST_EQUIV(A, B) \
	KAI_TEST_EQUIV_GIVEN(this, A,B)

#	define KAI_TEST_NOT_EQUIV(A, B) \
	KAI_TEST_NOT_EQUIV_GIVEN(this, A,B)

#	define KAI_TEST_NOT_EQUAL(A, B) \
	KAI_TEST_NOT_EQUAL_GIVEN(this, A, B) \

#	define KAI_TEST_EQUAL(A, B) \
	KAI_TEST_EQUAL_GIVEN(this, A, B)

#	define KAI_TEST_LESS(A, B) \
	KAI_TEST_LESS_GIVEN(this, A,B)

#	define KAI_TEST_LESS_OR_EQUIV(A, B) \
	KAI_TEST_LESS_OR_EQUIV_GIVEN(this, A,B)

#	define KAI_TEST_LESS_OR_EQUAL(A, B) \
	KAI_TEST_LESS_OR_EQUAL_GIVEN(this, A, B)

#	define KAI_TEST_NOT_GREATER(A, B) \
	KAI_TEST_NOT_GREATER_GIVEN(this, A, B)

#	define KAI_TEST_GREATER(A, B) \
	KAI_TEST_GREATER_GIVEN(this, A,B)

#	define KAI_TEST_NOT_IMPLEMENTED() \
	KAI_TEST_NOT_IMPLEMENTED_GIVEN(this)

#	define KAI_TEST_NOT_IMPLEMENTED_TEXT(T) \
	KAI_TEST_NOT_IMPLEMENTED_TEXT_GIVEN(this, T)

#	define KAI_TEST_DEPRECATED(T) \
	KAI_TEST_DEPRECATED_GIVEN(this, T)

#	define KAI_TEST_MESSAGE(T) \
	KAI_TEST_MESSAGE_GIVEN(this, T)

#endif // defined(KAI_UNIT_TESTS) && !defined(KAI_TEST_MACROS_H)

//EOF
