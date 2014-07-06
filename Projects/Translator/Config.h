#pragma once

//#define KAI_USE_MONOTONIC_ALLOCATOR

#include "KAI/Config/Base.h"

#ifndef KAI_TRANS_NAMESPACE_NAME
#	define KAI_TRANS_NAMESPACE_NAME trans
#endif

#define KAI_TRANS_BEGIN KAI_BEGIN namespace KAI_TRANS_NAMESPACE_NAME {

#define KAI_TRANS_END KAI_END }

