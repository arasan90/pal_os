#include <dlfcn.h>
#include <gtest/gtest.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pal_os/system.h"

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