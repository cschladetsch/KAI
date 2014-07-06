

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_MEMORY_BASE_ALLOCATOR_H
#	define KAI_MEMORY_BASE_ALLOCATOR_H

KAI_BEGIN

namespace Memory
{
	/// Commonality for all allocators
	struct BaseAllocator : IAllocator
	{
		typedef char Byte;
		typedef char *BytePtr;

		Allocator alloc;
		DeAllocator free;

		BaseAllocator() : alloc(0), free(0) 
		{ 
		}
		VoidPtr AllocateBytes(size_t N)
		{
			if (!alloc)
				return 0;
			return alloc(N);
		}
		void DeAllocateBytes(VoidPtr ptr, size_t num_bytes)
		{
			if (!free)
				return;
			free(ptr, num_bytes);
		}
	};

	/// Standard allocator - use ::malloc and ::free
	struct StandardAllocator : BaseAllocator
	{
		StandardAllocator()
		{
			alloc = &StandardAllocator::sys_malloc;
			free = &StandardAllocator::sys_free;
		}
		static VoidPtr sys_malloc(size_t N)
		{
			return ::malloc(N);
		}
		static void sys_free(VoidPtr P, size_t /*N*/)
		{
			return ::free(P);
		}
	};
}

KAI_END

#endif // KAI_MEMORY_BASE_ALLOCATOR_H

//EOF