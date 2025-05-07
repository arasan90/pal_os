#include <gtest/gtest.h>

#include "pal_os/time.h"

TEST(pal_os_time, unixAndSystemTimeSuccess)
{
	size_t unix_time = pal_os_get_unix_time();
	EXPECT_NE(unix_time, 0);

	size_t system_time = pal_os_get_system_time();
	EXPECT_NE(system_time, 0);

	EXPECT_GT(unix_time, system_time);
}