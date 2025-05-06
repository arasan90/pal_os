#include <dlfcn.h>
#include <gtest/gtest.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pal_os/system.h"

static bool fail_allocations = false;

extern "C" void *malloc(size_t size)
{
	if (fail_allocations)
	{
		fprintf(stderr, "[mock] malloc(%zu) -> NULL\n", size);
		return NULL;
	}

	using malloc_func_t				 = void *(*)(size_t);
	static malloc_func_t real_malloc = (malloc_func_t)dlsym(RTLD_NEXT, "malloc");
	return real_malloc(size);
}

extern "C" void *calloc(size_t num, size_t size)
{
	if (fail_allocations)
	{
		fprintf(stderr, "[mock] calloc(%zu) -> NULL\n", size);
		return NULL;
	}

	using calloc_func_t				 = void *(*)(size_t, size_t);
	static calloc_func_t real_calloc = (calloc_func_t)dlsym(RTLD_NEXT, "calloc");
	return real_calloc(num, size);
}

extern "C" void *realloc(void *ptr, size_t size)
{
	if (fail_allocations)
	{
		fprintf(stderr, "[mock] realloc(%zu) -> NULL\n", size);
		return NULL;
	}

	using realloc_func_t			   = void *(*)(void *, size_t);
	static realloc_func_t real_realloc = (realloc_func_t)dlsym(RTLD_NEXT, "realloc");
	return real_realloc(ptr, size);
}

TEST(pal_os_system, printOnStdOutSuccess)
{
	char  buffer[1024];
	FILE *stdout_backup = stdout;
	FILE *mem_stream	= fmemopen(buffer, sizeof(buffer), "w");
	stdout				= mem_stream;
	pal_system_printf("Hello, %s! The answer is always %d!\n", "world", 42);
	stdout = stdout_backup;
	fclose(mem_stream);
	EXPECT_STREQ(buffer, "Hello, world! The answer is always 42!\n");
}

TEST(pal_os_system, mallocSuccess)
{
	pal_system_t *stats = pal_system_get_stats();
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, 0);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, 0);
	void  *ptr	= pal_system_malloc(1024);
	size_t size = malloc_usable_size(ptr);
	EXPECT_NE(ptr, nullptr);
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, size);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, size);
	pal_system_free(ptr);
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, 0);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, size);
}

TEST(pal_os_system, mallocFail)
{
	pal_system_t *stats = pal_system_get_stats();
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, 0);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, 0);
	fail_allocations = true;
	void *ptr		 = pal_system_malloc(1024);
	fail_allocations = false;
	EXPECT_EQ(ptr, nullptr);
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, 0);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, 0);
	pal_system_free(ptr);
}

TEST(pal_os_system, callocSuccess)
{
	pal_system_t *stats = pal_system_get_stats();
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, 0);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, 0);
	void  *ptr	= pal_system_calloc(1, 1024);
	size_t size = malloc_usable_size(ptr);
	EXPECT_NE(ptr, nullptr);
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, size);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, size);
	for (int i = 0; i < 1024; i++)
	{
		EXPECT_EQ(((char *)ptr)[i], 0);
	}
	pal_system_free(ptr);
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, 0);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, size);
}

TEST(pal_os_system, callocFailure)
{
	pal_system_t *stats = pal_system_get_stats();
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, 0);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, 0);
	fail_allocations = true;
	void *ptr		 = pal_system_calloc(1, 1024);
	fail_allocations = false;
	size_t size		 = malloc_usable_size(ptr);
	EXPECT_EQ(ptr, nullptr);
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, 0);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, 0);
	pal_system_free(ptr);
}

TEST(pal_os_system, reallocSuccess)
{
	pal_system_t *stats = pal_system_get_stats();
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, 0);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, 0);
	void  *ptr	= pal_system_malloc(1024);
	size_t size = malloc_usable_size(ptr);
	EXPECT_NE(ptr, nullptr);
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, size);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, size);
	ptr	 = pal_system_realloc(ptr, 2048);
	size = malloc_usable_size(ptr);
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, size);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, size);
	ptr = pal_system_realloc(ptr, 1024);
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, malloc_usable_size(ptr));
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, size);
	pal_system_free(ptr);
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, 0);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, size);
}

TEST(pal_os_system, reallocNoPreviousAllocSuccess)
{
	pal_system_t *stats = pal_system_get_stats();
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, 0);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, 0);
	void  *ptr = ptr = pal_system_realloc(nullptr, 1024);
	size_t size		 = malloc_usable_size(ptr);
	EXPECT_NE(ptr, nullptr);
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, size);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, size);
	pal_system_free(ptr);
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, 0);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, size);
}

TEST(pal_os_system, reallocFail)
{
	pal_system_t *stats = pal_system_get_stats();
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, 0);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, 0);
	void  *ptr	= pal_system_malloc(1024);
	size_t size = malloc_usable_size(ptr);
	EXPECT_NE(ptr, nullptr);
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, size);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, size);
	fail_allocations = true;
	ptr				 = pal_system_realloc(ptr, 2048);
	fail_allocations = false;
	EXPECT_EQ(nullptr, ptr);
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, size);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, size);
	pal_system_free(ptr);
}

TEST(pal_os_system, freeSuccess)
{
	pal_system_t *stats = pal_system_get_stats();
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, 0);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, 0);
	void  *ptr	= pal_system_malloc(1024);
	size_t size = malloc_usable_size(ptr);
	EXPECT_NE(ptr, nullptr);
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, size);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, size);
	pal_system_free(ptr);
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, 0);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, size);
}

TEST(pal_os_system, freeNullSuccess)
{
	pal_system_t *stats = pal_system_get_stats();
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, 0);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, 0);
	pal_system_free(nullptr);
	EXPECT_EQ(stats->heap_size, 0);
	EXPECT_EQ(stats->used_heap_size, 0);
	EXPECT_EQ(stats->free_heap_size, 0);
	EXPECT_EQ(stats->max_used_heap_size, 0);
}