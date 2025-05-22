#include <gtest/gtest.h>

#include "pal_os/thread.h"
#include "thread_priv.h"

void thread_function(void *arg)
{
	int exit = NULL == arg ? 1 : *(int *)arg;
	while (!exit)
	{
		pal_thread_sleep(100);
	}
}

TEST(pal_os_thread, createThreadOkNullArg)
{
	pal_thread_t thread = {0};
	EXPECT_EQ(0, pal_thread_create(&thread, PAL_THREAD_PRIORITY_NORMAL, 1024, thread_function, "TestThread", NULL));
	EXPECT_EQ(PAL_THREAD_PRIORITY_NORMAL, thread.priority);
	EXPECT_EQ(1024, thread.stack_size);
	EXPECT_EQ(PAL_THREAD_STATE_RUNNING, thread.state);
	EXPECT_EQ(thread_function, thread.func);
	EXPECT_STREQ("TestThread", thread.name);
	EXPECT_EQ(NULL, thread.arg);
	EXPECT_NE(NULL, thread.thread);
	pal_thread_join(&thread);
	EXPECT_EQ(PAL_THREAD_STATE_TERMINATED, thread.state);
	pal_thread_free(&thread);
}

TEST(pal_os_thread, createThreadOkWithArg)
{
	int			 exit	= 0;
	pal_thread_t thread = {0};
	EXPECT_EQ(0, pal_thread_create(&thread, PAL_THREAD_PRIORITY_NORMAL, 1024, thread_function, "TestThread", &exit));
	EXPECT_EQ(PAL_THREAD_PRIORITY_NORMAL, thread.priority);
	EXPECT_EQ(1024, thread.stack_size);
	EXPECT_EQ(PAL_THREAD_STATE_RUNNING, thread.state);
	EXPECT_EQ(thread_function, thread.func);
	EXPECT_STREQ("TestThread", thread.name);
	EXPECT_EQ(&exit, thread.arg);
	EXPECT_NE(NULL, thread.thread);
	exit = 1;
	pal_thread_join(&thread);
	EXPECT_EQ(PAL_THREAD_STATE_TERMINATED, thread.state);
	pal_thread_free(&thread);
}

TEST(pal_os_thread, createThreadOkNoName)
{
	pal_thread_t thread = {0};
	EXPECT_EQ(0, pal_thread_create(&thread, PAL_THREAD_PRIORITY_NORMAL, 1024, thread_function, NULL, NULL));
	EXPECT_EQ(PAL_THREAD_PRIORITY_NORMAL, thread.priority);
	EXPECT_EQ(1024, thread.stack_size);
	EXPECT_EQ(PAL_THREAD_STATE_RUNNING, thread.state);
	EXPECT_EQ(thread_function, thread.func);
	EXPECT_EQ(NULL, thread.name);
	EXPECT_EQ(NULL, thread.arg);
	EXPECT_NE(NULL, thread.thread);
	pal_thread_join(&thread);
	EXPECT_EQ(PAL_THREAD_STATE_TERMINATED, thread.state);
	pal_thread_free(&thread);
}

TEST(pal_os_thread, createThreadOkEmptyName)
{
	pal_thread_t thread = {0};
	EXPECT_EQ(0, pal_thread_create(&thread, PAL_THREAD_PRIORITY_NORMAL, 1024, thread_function, "", NULL));
	EXPECT_EQ(PAL_THREAD_PRIORITY_NORMAL, thread.priority);
	EXPECT_EQ(1024, thread.stack_size);
	EXPECT_EQ(PAL_THREAD_STATE_RUNNING, thread.state);
	EXPECT_EQ(thread_function, thread.func);
	EXPECT_STREQ("", thread.name);
	EXPECT_EQ(NULL, thread.arg);
	EXPECT_NE(NULL, thread.thread);
	pal_thread_join(&thread);
	EXPECT_EQ(PAL_THREAD_STATE_TERMINATED, thread.state);
	pal_thread_free(&thread);
}

TEST(pal_os_thread, createThreadNullThreadhandle)
{
	EXPECT_EQ(-1, pal_thread_create(NULL, PAL_THREAD_PRIORITY_NORMAL, 1024, thread_function, "TestThread", NULL));
}

TEST(pal_os_thread, createThread0BytesStack)
{
	pal_thread_t thread = {0};
	EXPECT_EQ(-1, pal_thread_create(&thread, PAL_THREAD_PRIORITY_NORMAL, 0, thread_function, "TestThread", NULL));
}

TEST(pal_os_thread, createThreadNullFunction)
{
	pal_thread_t thread = {0};
	EXPECT_EQ(-1, pal_thread_create(&thread, PAL_THREAD_PRIORITY_NORMAL, 1024, nullptr, "TestThread", NULL));
}

TEST(pal_os_thread, threadSleep)
{
	time_t start_time = time(NULL);
	pal_thread_sleep(1000);
	time_t end_time = time(NULL);
	EXPECT_EQ(1, end_time - start_time);
}