#include <gtest/gtest.h>

#include "pal_os/common.h"
#include "pal_os/mutex.h"

TEST(pal_os_mutex, createMutexFailNullPointer)
{
	pal_mutex_t *mutex = NULL;
	EXPECT_EQ(-1, pal_mutex_create(nullptr, 0));
}

TEST(pal_os_mutex, createNonRecursiveMutexSuccess)
{
	pal_mutex_t mutex = {0};
	EXPECT_EQ(0, pal_mutex_create(&mutex, 0));
	EXPECT_EQ(0, pal_mutex_lock(&mutex, PAL_OS_NO_TIMEOUT));
	EXPECT_EQ(-1, pal_mutex_lock(&mutex, PAL_OS_NO_TIMEOUT));
	EXPECT_EQ(0, pal_mutex_unlock(&mutex));
	EXPECT_EQ(0, pal_mutex_lock(&mutex, PAL_OS_NO_TIMEOUT));
	EXPECT_EQ(0, pal_mutex_unlock(&mutex));
	EXPECT_EQ(0, pal_mutex_destroy(&mutex));
}

TEST(pal_os_mutex, createRecursiveMutexSuccess)
{
	pal_mutex_t mutex = {0};
	EXPECT_EQ(0, pal_mutex_create(&mutex, 1));
	EXPECT_EQ(0, pal_mutex_lock(&mutex, PAL_OS_NO_TIMEOUT));
	EXPECT_EQ(0, pal_mutex_lock(&mutex, PAL_OS_NO_TIMEOUT));
	EXPECT_EQ(0, pal_mutex_unlock(&mutex));
	EXPECT_EQ(0, pal_mutex_lock(&mutex, PAL_OS_NO_TIMEOUT));
	EXPECT_EQ(0, pal_mutex_unlock(&mutex));
	EXPECT_EQ(0, pal_mutex_unlock(&mutex));
	EXPECT_EQ(0, pal_mutex_destroy(&mutex));
}

TEST(pal_os_mutex, lockInfiniteTimeout)
{
	pal_mutex_t mutex = {0};
	EXPECT_EQ(0, pal_mutex_create(&mutex, 1));
	EXPECT_EQ(0, pal_mutex_lock(&mutex, PAL_OS_INFINITE_TIMEOUT));
	EXPECT_EQ(0, pal_mutex_unlock(&mutex));
	EXPECT_EQ(0, pal_mutex_destroy(&mutex));
}

TEST(pal_os_mutex, lockWithTimeoutFail)
{
	pal_mutex_t mutex	   = {0};
	time_t		start_time = 0;
	time_t		stop_time  = 0;
	EXPECT_EQ(0, pal_mutex_create(&mutex, 0));
	EXPECT_EQ(0, pal_mutex_lock(&mutex, PAL_OS_NO_TIMEOUT));
	start_time = time(NULL);
	EXPECT_EQ(-1, pal_mutex_lock(&mutex, 1000));
	stop_time = time(NULL);
	EXPECT_EQ(1, stop_time - start_time);
	EXPECT_EQ(0, pal_mutex_unlock(&mutex));
	EXPECT_EQ(0, pal_mutex_destroy(&mutex));
}