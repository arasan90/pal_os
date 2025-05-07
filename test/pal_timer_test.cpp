#include <gtest/gtest.h>

#include "timer_priv.h"

TEST(pal_timer, compareTimesNegativeResult)
{
	struct timespec time1 = {1, 0};
	struct timespec time2 = {2, 0};
	EXPECT_LT(pal_os_timer_time_cmp(&time1, &time2), 0);
}

TEST(pal_timer, compareTimesPositiveResult)
{
	struct timespec time1 = {1, 0};
	struct timespec time2 = {2, 0};
	EXPECT_GT(pal_os_timer_time_cmp(&time2, &time1), 0);
}

TEST(pal_timer, compareSameTimes)
{
	struct timespec time1 = {1, 0};
	EXPECT_EQ(0, pal_os_timer_time_cmp(&time1, &time1));
}

TEST(pal_timer, killThreadNoTimers)
{
	EXPECT_EQ(0, pal_timer_init());
	EXPECT_EQ(0, pal_timer_environment.shutdown_flag);
	EXPECT_NE(0, pal_timer_environment.thread_handle);
	EXPECT_EQ(nullptr, pal_timer_environment.timer_list);
	pal_timer_deinit();
	EXPECT_EQ(0, pal_timer_environment.thread_handle);
}

TEST(pal_timer, add3TimersSorted)
{
	pal_timer_environment.timer_list = nullptr;
	EXPECT_EQ(nullptr, pal_timer_environment.timer_list);

	pal_timer_t *timer1 = (pal_timer_t *)malloc(sizeof(pal_timer_t));
	pal_timer_t *timer2 = (pal_timer_t *)malloc(sizeof(pal_timer_t));
	pal_timer_t *timer3 = (pal_timer_t *)malloc(sizeof(pal_timer_t));

	timer1->expiry_time.tv_sec	= 1;
	timer1->expiry_time.tv_nsec = 0;
	timer2->expiry_time.tv_sec	= 2;
	timer2->expiry_time.tv_nsec = 0;
	timer3->expiry_time.tv_sec	= 3;
	timer3->expiry_time.tv_nsec = 0;

	pal_os_timer_insert_sorted(timer1);
	EXPECT_EQ(timer1, pal_timer_environment.timer_list);

	pal_os_timer_insert_sorted(timer2);
	EXPECT_EQ(timer1, pal_timer_environment.timer_list);
	EXPECT_EQ(timer2, timer1->next);

	pal_os_timer_insert_sorted(timer3);
	EXPECT_EQ(timer1, pal_timer_environment.timer_list);
	EXPECT_EQ(timer2, timer1->next);
	EXPECT_EQ(timer3, timer2->next);

	free(timer1);
	free(timer2);
	free(timer3);
}

TEST(pal_timer, add5TimersNotSorted)
{
	pal_timer_environment.timer_list = nullptr;
	EXPECT_EQ(nullptr, pal_timer_environment.timer_list);

	pal_timer_t *timer1 = (pal_timer_t *)calloc(1, sizeof(pal_timer_t));
	pal_timer_t *timer2 = (pal_timer_t *)calloc(1, sizeof(pal_timer_t));
	pal_timer_t *timer3 = (pal_timer_t *)calloc(1, sizeof(pal_timer_t));
	pal_timer_t *timer4 = (pal_timer_t *)calloc(1, sizeof(pal_timer_t));
	pal_timer_t *timer5 = (pal_timer_t *)calloc(1, sizeof(pal_timer_t));

	timer1->expiry_time.tv_sec	= 1;
	timer1->expiry_time.tv_nsec = 5241;
	timer2->expiry_time.tv_sec	= 2;
	timer2->expiry_time.tv_nsec = 215141;
	timer3->expiry_time.tv_sec	= 3;
	timer3->expiry_time.tv_nsec = 321312312;
	timer4->expiry_time.tv_sec	= 3;
	timer4->expiry_time.tv_nsec = 214;
	timer5->expiry_time.tv_sec	= 1;
	timer5->expiry_time.tv_nsec = 425252;

	pal_os_timer_insert_sorted(timer3);
	pal_os_timer_insert_sorted(timer2);
	pal_os_timer_insert_sorted(timer5);
	pal_os_timer_insert_sorted(timer1);
	pal_os_timer_insert_sorted(timer4);

	EXPECT_EQ(timer1, pal_timer_environment.timer_list);
	EXPECT_EQ(timer5, timer1->next);
	EXPECT_EQ(timer2, timer5->next);
	EXPECT_EQ(timer4, timer2->next);
	EXPECT_EQ(timer3, timer4->next);
	free(timer1);
	free(timer2);
	free(timer3);
	free(timer4);
	free(timer5);
}

TEST(pal_timer, killThreadWithTimers)
{
	pal_timer_environment.timer_list = nullptr;
	EXPECT_EQ(nullptr, pal_timer_environment.timer_list);

	pal_timer_t *timer1 = (pal_timer_t *)calloc(1, sizeof(pal_timer_t));
	pal_timer_t *timer2 = (pal_timer_t *)calloc(1, sizeof(pal_timer_t));
	pal_timer_t *timer3 = (pal_timer_t *)calloc(1, sizeof(pal_timer_t));
	pal_timer_t *timer4 = (pal_timer_t *)calloc(1, sizeof(pal_timer_t));
	pal_timer_t *timer5 = (pal_timer_t *)calloc(1, sizeof(pal_timer_t));

	timer1->expiry_time.tv_sec	= 1;
	timer1->expiry_time.tv_nsec = 5241;
	timer2->expiry_time.tv_sec	= 2;
	timer2->expiry_time.tv_nsec = 215141;
	timer3->expiry_time.tv_sec	= 3;
	timer3->expiry_time.tv_nsec = 321312312;
	timer4->expiry_time.tv_sec	= 3;
	timer4->expiry_time.tv_nsec = 214;
	timer5->expiry_time.tv_sec	= 1;
	timer5->expiry_time.tv_nsec = 425252;

	pal_os_timer_insert_sorted(timer2);
	pal_os_timer_insert_sorted(timer3);
	pal_os_timer_insert_sorted(timer5);
	pal_os_timer_insert_sorted(timer1);
	pal_os_timer_insert_sorted(timer4);

	EXPECT_EQ(timer1, pal_timer_environment.timer_list);
	EXPECT_EQ(timer5, timer1->next);
	EXPECT_EQ(timer2, timer5->next);
	EXPECT_EQ(timer4, timer2->next);
	EXPECT_EQ(timer3, timer4->next);
	pal_timer_init();
	EXPECT_NE(0, pal_timer_environment.thread_handle);
	pal_timer_deinit();
	EXPECT_EQ(0, pal_timer_environment.thread_handle);
	EXPECT_EQ(nullptr, pal_timer_environment.timer_list);
}