#include <gtest/gtest.h>

#include "pal_os/thread.h"
#include "pal_os/thread_priv.h"

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
	pal_thread_t *thread = NULL;
	EXPECT_EQ(0, pal_thread_create(&thread, PAL_THREAD_PRIORITY_NORMAL, 1024, thread_function, "TestThread", NULL));
	struct pal_thread_s *thread_struct = thread;
	EXPECT_EQ(PAL_THREAD_PRIORITY_NORMAL, thread_struct->priority);
	EXPECT_EQ(1024, thread_struct->stack_size);
	EXPECT_EQ(PAL_THREAD_STATE_RUNNING, thread_struct->state);
	EXPECT_EQ(thread_function, thread_struct->func);
	EXPECT_STREQ("TestThread", thread_struct->name);
	EXPECT_EQ(NULL, thread_struct->arg);
	EXPECT_NE(NULL, thread_struct->thread);
	pal_thread_join(thread);
	EXPECT_EQ(PAL_THREAD_STATE_TERMINATED, thread_struct->state);
	pal_thread_free(&thread);
	EXPECT_EQ(NULL, thread);
}

TEST(pal_os_thread, createThreadOkWithArg)
{
	int exit = 0;
	pal_thread_t *thread = NULL;
	EXPECT_EQ(0, pal_thread_create(&thread, PAL_THREAD_PRIORITY_NORMAL, 1024, thread_function, "TestThread", &exit));
	struct pal_thread_s *thread_struct = thread;
	EXPECT_EQ(PAL_THREAD_PRIORITY_NORMAL, thread_struct->priority);
	EXPECT_EQ(1024, thread_struct->stack_size);
	EXPECT_EQ(PAL_THREAD_STATE_RUNNING, thread_struct->state);
	EXPECT_EQ(thread_function, thread_struct->func);
	EXPECT_STREQ("TestThread", thread_struct->name);
	EXPECT_EQ(&exit, thread_struct->arg);
	EXPECT_NE(NULL, thread_struct->thread);
	exit = 1;
	pal_thread_join(thread);
	EXPECT_EQ(PAL_THREAD_STATE_TERMINATED, thread_struct->state);
	pal_thread_free(&thread);
}

TEST(pal_os_thread, createThreadOkNoName)
{
	pal_thread_t *thread = NULL;
	EXPECT_EQ(0, pal_thread_create(&thread, PAL_THREAD_PRIORITY_NORMAL, 1024, thread_function, NULL, NULL));
	struct pal_thread_s *thread_struct = thread;
	EXPECT_EQ(PAL_THREAD_PRIORITY_NORMAL, thread_struct->priority);
	EXPECT_EQ(1024, thread_struct->stack_size);
	EXPECT_EQ(PAL_THREAD_STATE_RUNNING, thread_struct->state);
	EXPECT_EQ(thread_function, thread_struct->func);
	EXPECT_EQ(NULL, thread_struct->name);
	EXPECT_EQ(NULL, thread_struct->arg);
	EXPECT_NE(NULL, thread_struct->thread);
	pal_thread_join(thread);
	EXPECT_EQ(PAL_THREAD_STATE_TERMINATED, thread_struct->state);
	pal_thread_free(&thread);
	EXPECT_EQ(NULL, thread);
}

TEST(pal_os_thread, createThreadOkEmptyName)
{
	pal_thread_t *thread = NULL;
	EXPECT_EQ(0, pal_thread_create(&thread, PAL_THREAD_PRIORITY_NORMAL, 1024, thread_function, "", NULL));
	struct pal_thread_s *thread_struct = thread;
	EXPECT_EQ(PAL_THREAD_PRIORITY_NORMAL, thread_struct->priority);
	EXPECT_EQ(1024, thread_struct->stack_size);
	EXPECT_EQ(PAL_THREAD_STATE_RUNNING, thread_struct->state);
	EXPECT_EQ(thread_function, thread_struct->func);
	EXPECT_STREQ("", thread_struct->name);
	EXPECT_EQ(NULL, thread_struct->arg);
	EXPECT_NE(NULL, thread_struct->thread);
	pal_thread_join(thread);
	EXPECT_EQ(PAL_THREAD_STATE_TERMINATED, thread_struct->state);
	pal_thread_free(&thread);
	EXPECT_EQ(NULL, thread);
}

TEST(pal_os_thread, createThreadNullThreadhandle)
{
	EXPECT_EQ(-1, pal_thread_create(NULL, PAL_THREAD_PRIORITY_NORMAL, 1024, thread_function, "TestThread", NULL));
}

TEST(pal_os_thread, createThread0BytesStack)
{
	pal_thread_t *thread = NULL;
	EXPECT_EQ(-1, pal_thread_create(&thread, PAL_THREAD_PRIORITY_NORMAL, 0, thread_function, "TestThread", NULL));
}

TEST(pal_os_thread, createThreadNullFunction)
{
	pal_thread_t *thread = NULL;
	EXPECT_EQ(-1, pal_thread_create(&thread, PAL_THREAD_PRIORITY_NORMAL, 0, NULL, "TestThread", NULL));
	struct pal_thread_s *thread_struct = thread;
	EXPECT_EQ(NULL, thread_struct);
}

TEST(pal_os_thread, threadSleep)
{
	time_t start_time = time(NULL);
	pal_thread_sleep(1000);
	time_t end_time = time(NULL);
	EXPECT_EQ(1, end_time - start_time);
}