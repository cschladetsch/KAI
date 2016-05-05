#pragma once

// defined to place the system into a single namespace
#define KAI_OPTION_USE_NAMESPACES

// if KAI_OPTION_USE_NAMESPACES is defined, this is the namespace name to use
//#define KAI_NAMESPACE_NAME kai
#ifndef KAI_NAMESPACE_NAME
#warning please define KAI_NAMESPACE_NAME
#define KAI_NAMESPACE_NAME kai
#endif

//  use lots of extra info in trace mesages
//#define KAI_TRACE_VERBOSE

// generally more safety and checking
//#define KAI_DEBUG

// enable tracing at all
//#define KAI_DEBUG_TRACE

// use exceptions or not
//#define KAI_USE_EXCEPTIONS

// deprecated
//#define KAI_UNIT_TESTS

// extra tracing for Registry/Garbage collection system
//#define KAI_TRACE_REGISTRY

// if KAI_DEBUG is defined, extra debugging features may be exposed
#if defined(_DEBUG) || defined(DEBUG)
#	ifndef KAI_DEBUG
#		define KAI_DEBUG
#	endif
#endif

#ifdef KAI_DEBUG
// if KAI_OBJECT_HAS_STORAGEBASE is defined, a pointer to the value of the 
// stored inside it with the name 'storage'. this is entirely for debugging 
// purposes.
#	define KAI_OBJECT_HAS_STORAGEBASE
#	define KAI_POINTER_HAS_STORAGEBASE

// store pointer to referent on assign
#	define	KAI_OPTION_DEBUG_SAVE_POINTER
#endif

#ifndef UNUSED
#	define UNUSED(X) (void)(sizeof(X))
#endif

