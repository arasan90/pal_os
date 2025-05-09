#include <gtest/gtest.h>

#include "timer_priv.h"

static int timerCounter = 0;
void	   timerCallback(pal_timer_t *arg) { timerCounter++; }

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
	EXPECT_EQ(nullptr, timer3->next);

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
	EXPECT_EQ(nullptr, timer3->next);
	free(timer1);
	free(timer2);
	free(timer3);
	free(timer4);
	free(timer5);
}

TEST(pal_timer, removeTimer)
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
	EXPECT_EQ(nullptr, timer3->next);

	pal_os_timer_remove(timer3);

	EXPECT_EQ(timer1, pal_timer_environment.timer_list);
	EXPECT_EQ(timer5, timer1->next);
	EXPECT_EQ(timer2, timer5->next);
	EXPECT_EQ(timer4, timer2->next);
	EXPECT_EQ(nullptr, timer4->next);

	pal_os_timer_remove(timer2);

	EXPECT_EQ(timer1, pal_timer_environment.timer_list);
	EXPECT_EQ(timer5, timer1->next);
	EXPECT_EQ(timer4, timer5->next);
	EXPECT_EQ(nullptr, timer4->next);

	pal_os_timer_remove(timer1);

	EXPECT_EQ(timer5, pal_timer_environment.timer_list);
	EXPECT_EQ(timer4, timer5->next);
	EXPECT_EQ(nullptr, timer4->next);

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

TEST(pal_timer, createTimerAutoStartOneShot)
{
	timerCounter					 = 0;
	pal_timer_environment.timer_list = nullptr;
	pal_timer_t *timer				 = nullptr;
	pal_timer_init();
	EXPECT_EQ(0, pal_timer_create(&timer, "", PAL_TIMER_TYPE_ONESHOT, 300, timerCallback, 1, nullptr));
	EXPECT_NE(nullptr, timer);
	sleep(1);
	EXPECT_EQ(1, timerCounter);
	EXPECT_EQ(nullptr, pal_timer_environment.timer_list);
	pal_timer_deinit();
	free(timer);
}

TEST(pal_timer, createTimerAutoStartPeriodic)
{
	timerCounter					 = 0;
	pal_timer_environment.timer_list = nullptr;
	pal_timer_t *timer				 = nullptr;
	pal_timer_init();
	EXPECT_EQ(0, pal_timer_create(&timer, "", PAL_TIMER_TYPE_PERIODIC, 300, timerCallback, 1, nullptr));
	EXPECT_NE(nullptr, timer);
	sleep(1);
	EXPECT_EQ(3, timerCounter);
	EXPECT_NE(nullptr, pal_timer_environment.timer_list);
	pal_timer_deinit();
}

TEST(pal_timer, createTimerNoAutoStartOneShot)
{
	timerCounter						= 0;
	pal_timer_environment.timer_list	= nullptr;
	pal_timer_environment.shutdown_flag = 0;
	pal_timer_t *timer					= nullptr;
	pal_timer_init();
	EXPECT_EQ(0, pal_timer_create(&timer, "", PAL_TIMER_TYPE_ONESHOT, 100, timerCallback, 0, nullptr));
	EXPECT_NE(nullptr, timer);
	sleep(1);
	EXPECT_EQ(0, timerCounter);
	EXPECT_EQ(0, pal_timer_start(timer, 0));
	sleep(1);
	EXPECT_EQ(1, timerCounter);
	EXPECT_EQ(nullptr, pal_timer_environment.timer_list);
	pal_timer_deinit();
	free(timer);
}

TEST(pal_timer, stopPeriodicTimer)
{
	timerCounter						= 0;
	pal_timer_environment.timer_list	= nullptr;
	pal_timer_environment.shutdown_flag = 0;
	pal_timer_t *timer					= nullptr;
	pal_timer_init();
	EXPECT_EQ(0, pal_timer_create(&timer, "", PAL_TIMER_TYPE_PERIODIC, 300, timerCallback, 0, nullptr));
	EXPECT_NE(nullptr, timer);
	sleep(1);
	EXPECT_EQ(0, timerCounter);
	EXPECT_EQ(0, pal_timer_start(timer, 0));
	sleep(1);
	EXPECT_EQ(3, timerCounter);
	EXPECT_EQ(0, pal_timer_stop(timer, 0));
	EXPECT_EQ(nullptr, pal_timer_environment.timer_list);
	sleep(1);
	EXPECT_EQ(3, timerCounter);
	EXPECT_EQ(0, pal_timer_start(timer, 0));
	sleep(1);
	EXPECT_EQ(6, timerCounter);
	EXPECT_EQ(0, pal_timer_stop(timer, 0));
	pal_timer_deinit();
	free(timer);
}

TEST(pal_timer, stopTimerNullPtrFailure) { EXPECT_EQ(-1, pal_timer_stop(nullptr, 0)); }

TEST(pal_timer, stopTimerNotCreatedFailure)
{
	pal_timer_t *timer = nullptr;
	EXPECT_EQ(-1, pal_timer_stop(timer, 0));
}

TEST(pal_timer, stopNotStartedTimerSuccess)
{
	pal_timer_t *timer = nullptr;
	pal_timer_create(&timer, "", PAL_TIMER_TYPE_PERIODIC, 300, timerCallback, 0, nullptr);
	EXPECT_EQ(0, pal_timer_stop(timer, 0));
	free(timer);
}

TEST(pal_timer, startTimerNullPtrFailure) { EXPECT_EQ(-1, pal_timer_start(nullptr, 0)); }

TEST(pal_timer, startTimerNotCreatedFailure)
{
	pal_timer_t *timer = nullptr;
	EXPECT_EQ(-1, pal_timer_start(timer, 0));
}

TEST(pal_timer, restartOneShotTimer)
{
	timerCounter						= 0;
	pal_timer_environment.timer_list	= nullptr;
	pal_timer_environment.shutdown_flag = 0;
	pal_timer_t *timer					= nullptr;
	pal_timer_init();
	EXPECT_EQ(0, pal_timer_create(&timer, "", PAL_TIMER_TYPE_ONESHOT, 100, timerCallback, 0, nullptr));
	EXPECT_NE(nullptr, timer);
	pal_timer_start(timer, 0);
	sleep(1);
	EXPECT_EQ(1, timerCounter);
	EXPECT_EQ(0, pal_timer_restart(timer, 0));
	sleep(1);
	EXPECT_EQ(2, timerCounter);
	pal_timer_deinit();
	free(timer);
}

TEST(pal_timer, changePeriodOneShotTimer)
{
	timerCounter						= 0;
	pal_timer_environment.timer_list	= nullptr;
	pal_timer_environment.shutdown_flag = 0;
	pal_timer_t *timer					= nullptr;
	pal_timer_init();
	EXPECT_EQ(0, pal_timer_create(&timer, "", PAL_TIMER_TYPE_ONESHOT, 100, timerCallback, 1, nullptr));
	sleep(1);
	EXPECT_EQ(1, timerCounter);
	EXPECT_EQ(0, pal_timer_change_period(timer, 200, 0));
	sleep(1);
	EXPECT_EQ(2, timerCounter);
	pal_timer_deinit();
	free(timer);
}

TEST(pal_timer, changePeriodWith0Failure)
{
	pal_timer_t *timer				 = nullptr;
	pal_timer_environment.timer_list = nullptr;
	EXPECT_EQ(0, pal_timer_create(&timer, "", PAL_TIMER_TYPE_PERIODIC, 300, timerCallback, 1, nullptr));
	EXPECT_EQ(-1, pal_timer_change_period(timer, 0, 0));
	free(timer);
}

TEST(pal_timer, changePeriodWithNullPtrFailure)
{
	pal_timer_environment.timer_list = nullptr;
	EXPECT_EQ(-1, pal_timer_change_period(nullptr, 300, 0));
}

TEST(pal_timer, changePeriodWithNotCreatedFailure)
{
	pal_timer_t *timer				 = nullptr;
	pal_timer_environment.timer_list = nullptr;
	EXPECT_EQ(-1, pal_timer_change_period(timer, 300, 0));
}

TEST(pal_timer, isTimerActiveNoAutoStart)
{
	pal_timer_environment.timer_list = nullptr;
	pal_timer_t *timer				 = nullptr;
	EXPECT_EQ(0, pal_timer_create(&timer, "", PAL_TIMER_TYPE_PERIODIC, 300, timerCallback, 0, nullptr));
	EXPECT_EQ(0, pal_is_timer_active(timer));
	EXPECT_EQ(0, pal_timer_start(timer, 0));
	EXPECT_EQ(1, pal_is_timer_active(timer));
	pal_timer_deinit();
	free(timer);
}

TEST(pal_timer, isTimerActiveAutoStart)
{
	pal_timer_t *timer				 = nullptr;
	pal_timer_environment.timer_list = nullptr;
	EXPECT_EQ(0, pal_timer_create(&timer, "", PAL_TIMER_TYPE_PERIODIC, 300, timerCallback, 1, nullptr));
	EXPECT_EQ(1, pal_is_timer_active(timer));
	EXPECT_EQ(0, pal_timer_stop(timer, 0));
	EXPECT_EQ(0, pal_is_timer_active(timer));
	EXPECT_EQ(0, pal_timer_restart(timer, 0));
	EXPECT_EQ(1, pal_is_timer_active(timer));
	pal_timer_deinit();
	free(timer);
}

TEST(pal_timer, timerDeleteSuccess)
{
	pal_timer_t *timer				 = nullptr;
	pal_timer_environment.timer_list = nullptr;
	EXPECT_EQ(0, pal_timer_create(&timer, "", PAL_TIMER_TYPE_PERIODIC, 300, timerCallback, 1, nullptr));
	EXPECT_NE(nullptr, timer);
	EXPECT_EQ(0, pal_timer_delete(&timer));
	EXPECT_EQ(nullptr, timer);
}

TEST(pal_timer, timerDeleteNullPtrFailure)
{
	pal_timer_environment.timer_list = nullptr;
	EXPECT_EQ(-1, pal_timer_delete(nullptr));
}

TEST(pal_timer, timerDeleteNotCreatedFailure)
{
	pal_timer_t *timer				 = nullptr;
	pal_timer_environment.timer_list = nullptr;
	EXPECT_EQ(-1, pal_timer_delete(&timer));
	EXPECT_EQ(nullptr, timer);
}