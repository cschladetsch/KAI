#pragma once

#include <KAI/Core/Config/Base.h>

#include <cstdlib>

#include "BaseAllocator.h"
#include "KAI/Core/Memory/BaseAllocator.h"

KAI_BEGIN

namespace Memory {
/// Standard _allocator - use ::malloc and ::free
struct StandardAllocator : BaseAllocator {
    using BaseAllocator::alloc;
    using BaseAllocator::free;

    StandardAllocator() {
        alloc = &StandardAllocator::sys_malloc;
        free = &StandardAllocator::sys_free;
    }

    static void *sys_malloc(std::size_t N) { return ::malloc(N); }

    static void sys_free(void *P, std::size_t /*N*/) { return ::free(P); }
};
}  // namespace Memory

KAI_END
