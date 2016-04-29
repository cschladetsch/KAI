#pragma once

// defined to place the system into a single namespace
#define KAI_OPTION_USE_NAMESPACES

// if KAI_OPTION_USE_NAMESPACES is defined, this is the namespace name to use
//#define KAI_NAMESPACE_NAME kai
#ifndef KAI_NAMESPACE_NAME
#error please define KAI_NAMESPACE_NAME
#endif

//#define KAI_DEBUG
//#define KAI_USE_EXCEPTIONS

//#define KAI_UNIT_TESTS

/// if KAI_DEBUG is defined, extra debugging features may be exposed
#if defined(_DEBUG) || defined(DEBUG)
#	ifndef KAI_DEBUG
#		define KAI_DEBUG
#	endif
#endif

#ifdef KAI_DEBUG
#	ifndef KAI_DEBUG_TRACE
#		define KAI_DEBUG_TRACE
#	endif
#	define KAI_DEBUG_REGISTRY

/// if KAI_OBJECT_HAS_STORAGEBASE is defined, a pointer to the value of the 
/// stored inside it with the name 'storage'. this is entirely for debugging 
/// purposes.
#	define KAI_OBJECT_HAS_STORAGEBASE
#	define KAI_POINTER_HAS_STORAGEBASE

/// store pointer to referent on assign
#	define	KAI_OPTION_DEBUG_SAVE_POINTER

#endif

#ifndef UNUSED
#	define UNUSED(X) (void)(sizeof(X))
#endif

#if !defined(KAI_DEBUG_TRACE) && defined(KAI_DEBUG)
#	define KAI_DEBUG_TRACE
#endif
