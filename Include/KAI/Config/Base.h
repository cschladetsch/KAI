
#ifndef KAI_CONFIG_BASE_H
#	define KAI_CONFIG_BASE_H

//#	include "boost/config.hpp"
#	include "KAI/Config/Options.h"
#	include "KAI/Config/Compiler.h"

#	ifdef KAI_OPTION_USE_NAMESPACES
#		define KAI_BEGIN namespace KAI_NAMESPACE_NAME {
#		define KAI_END }
#		define USING_NAMESPACE_KAI using namespace KAI_NAMESPACE_NAME;
#		define KAI_NAMESPACE(E) ::KAI_NAMESPACE_NAME::E
#	else
#		undef KAI_NAMESPACE_NAME
#		define KAI_NAMESPACE(E) ::E
#		define KAI_BEGIN
#		define KAI_END
#		define USING_NAMESPACE_KAI
#	endif

#	ifdef KAI_OPTION_UNIT_TESTS
#		define KAI_TEST(ClassName) \
		KAI_PP_CONCAT("KAI/Tests/", \
			KAI_PP_CONCAT(KAI_PP_STRINGISE(ClassName), "TestSuite.h"))
#	else
#		define KAI_TEST(T) "KAI/Tests/DummyTestHeader.h"
#	endif

// use gc-count based caching system for object validity and value lookups
#define KAI_CACHE_OBJECT_LOOKUPS

//#	define KAI_PLATFORM_INCLUDE(Header) \
//		KAI_PP_CONCAT( \
//			KAI_PP_CONCAT("KAI/Platform", KAI_PP_STRINGISE(KAI_PLATFORM_LABEL)) \
//			, KAI_PP_STRINGISE(Header))
//
//#	define KAI_PLATFORM_Source(Source) \
//		KAI_PP_CONCAT( \
//			KAI_PP_CONCAT("./Platform", KAI_PP_STRINGISE(KAI_PLATFORM_LABEL)) \
//			, KAI_PP_STRINGISE(Header))

#	define KAI_UNUSED(A) ( &reinterpret_cast< const int& >( A ) )
#	define KAI_UNUSED_1(A) KAI_UNUSED(A)
#	define KAI_UNUSED_2(A,B) KAI_UNUSED(A), KAI_UNUSED(B)
#	define KAI_UNUSED_3(A,B,C) KAI_UNUSED(A), KAI_UNUSED(B), KAI_UNUSED(C)

#	include "KAI/Config/Platform.h"
#	include "KAI/Config/IntegralTypes.h"
#	include "KAI/Utility.h"

#endif // KAI_CONFIG_BASE_H

//EOF
