#include <gtest/gtest.h>

#include <thread>

#include "pal_os/common.h"
#include "pal_os/signal.h"

TEST(pal_os_signal, createSignalSuccess)
{
	pal_signal_t signal = {0};
	EXPECT_EQ(0, pal_signal_create(&signal));
	pal_signal_destroy(&signal);
}

TEST(pal_os_signal, createSignalFailure)
{
	pal_signal_t signal = {0};
	EXPECT_EQ(-1, pal_signal_create(nullptr));
}

TEST(pal_os_signal, destroySignalSuccess)
{
	pal_signal_t signal = {0};
	pal_signal_create(&signal);
	EXPECT_EQ(0, pal_signal_destroy(&signal));
}

TEST(pal_os_signal, destroySignalNullPtrFailure)
{
	pal_signal_t signal = {0};
	EXPECT_EQ(-1, pal_signal_destroy(nullptr));
}

TEST(pal_os_signal, setSignalNullPtrFailure) { EXPECT_EQ(-1, pal_signal_set(nullptr, 1, 0)); }

TEST(pal_os_signal, setSignalSuccess)
{
	pal_signal_t signal = {0};
	pal_signal_create(&signal);
	EXPECT_EQ(0, pal_signal_set(&signal, (1 << 3), 0));
	EXPECT_EQ((1 << 3), signal.signals);
	EXPECT_EQ(0, pal_signal_set(&signal, (1 << 1), 0));
	EXPECT_EQ((1 << 3) | (1 << 1), signal.signals);
	pal_signal_destroy(&signal);
}

TEST(pal_os_signal, ClearNullPtrFailure) { EXPECT_EQ(-1, pal_signal_clear(nullptr, 1)); }

TEST(pal_os_signal, ClearSuccess)
{
	pal_signal_t signal = {0};
	pal_signal_create(&signal);
	pal_signal_set(&signal, (1 << 3), 0);
	EXPECT_EQ(0, pal_signal_clear(&signal, (1 << 3)));
	EXPECT_EQ(0, signal.signals);
	pal_signal_destroy(&signal);
}

TEST(pal_os_signal, WaitNullPtrFailure)
{
	pal_signal_t signal			  = {0};
	size_t		 received_signals = 0;
	EXPECT_EQ(PAL_SIGNAL_FAILURE, pal_signal_wait(nullptr, (1 << 3), &received_signals, 0, 0, PAL_OS_NO_TIMEOUT));
	EXPECT_EQ(0, received_signals);
}

TEST(pal_os_signal, WaitNullReceivedSignalsFailure)
{
	pal_signal_t signal = {0};
	pal_signal_create(&signal);
	EXPECT_EQ(PAL_SIGNAL_FAILURE, pal_signal_wait(&signal, (1 << 3), nullptr, 0, 0, PAL_OS_NO_TIMEOUT));
	pal_signal_destroy(&signal);
}

TEST(pal_os_signal, WaitNoTimeoutSuccess)
{
	pal_signal_t signal			  = {0};
	size_t		 received_signals = 0;
	pal_signal_create(&signal);
	pal_signal_set(&signal, (1 << 3), 0);
	EXPECT_EQ(PAL_SIGNAL_SUCCESS, pal_signal_wait(&signal, (1 << 3), &received_signals, 0, 0, PAL_OS_NO_TIMEOUT));
	EXPECT_EQ((1 << 3), received_signals);
	EXPECT_EQ(signal.signals, (1 << 3));
	pal_signal_clear(&signal, (1 << 3));
	pal_signal_set(&signal, (1 << 3), 0);
	received_signals = 0;
	EXPECT_EQ(PAL_SIGNAL_SUCCESS, pal_signal_wait(&signal, (1 << 3), &received_signals, 1, 0, PAL_OS_NO_TIMEOUT));
	EXPECT_EQ((1 << 3), received_signals);
	EXPECT_EQ(signal.signals, 0);
	pal_signal_set(&signal, (1 << 3), 0);
	EXPECT_EQ(PAL_SIGNAL_SUCCESS, pal_signal_wait(&signal, (1 << 3) | (1 << 4), &received_signals, 1, 0, PAL_OS_NO_TIMEOUT));
	EXPECT_EQ((1 << 3), received_signals);
	EXPECT_EQ(signal.signals, 0);
	pal_signal_destroy(&signal);
}

TEST(pal_os_signal, WaitForAllSuccess)
{
	pal_signal_t signal			  = {0};
	size_t		 received_signals = 0;
	pal_signal_create(&signal);
	pal_signal_set(&signal, (1 << 3), 0);
	EXPECT_EQ(PAL_SIGNAL_TIMEOUT, pal_signal_wait(&signal, (1 << 3) | (1 << 4), &received_signals, 0, 1, PAL_OS_NO_TIMEOUT));
	EXPECT_EQ((1 << 3), received_signals);
	EXPECT_EQ(signal.signals, (1 << 3));
	pal_signal_set(&signal, (1 << 4), 0);
	EXPECT_EQ(PAL_SIGNAL_SUCCESS, pal_signal_wait(&signal, (1 << 3) | (1 << 4), &received_signals, 0, 1, PAL_OS_NO_TIMEOUT));
	EXPECT_EQ((1 << 3) | (1 << 4), received_signals);
	EXPECT_EQ(signal.signals, (1 << 3) | (1 << 4));
	pal_signal_destroy(&signal);
}

TEST(pal_os_signal, WaitInfiniteTimeoutSuccess)
{
	pal_signal_t signal			  = {0};
	size_t		 received_signals = 0;
	pal_signal_create(&signal);

	// Create a thread to set the signal after a delay
	std::thread signal_setter(
		[&]()
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			pal_signal_set(&signal, (1 << 2), 0);
		});

	// Wait for the signal with infinite timeout
	EXPECT_EQ(PAL_SIGNAL_SUCCESS, pal_signal_wait(&signal, (1 << 2), &received_signals, 0, 0, PAL_OS_INFINITE_TIMEOUT));
	EXPECT_EQ((1 << 2), received_signals);
	EXPECT_EQ(signal.signals, (1 << 2));

	// Join the thread and clean up
	signal_setter.join();
	pal_signal_destroy(&signal);
}
TEST(pal_os_signal, WaitFiniteTimeoutSuccessBeforeWait)
{
	pal_signal_t signal			  = {0};
	size_t		 received_signals = 0;
	pal_signal_create(&signal);

	// Set the signal before waiting
	pal_signal_set(&signal, (1 << 1), 0);

	// Wait for the signal with a finite timeout
	EXPECT_EQ(PAL_SIGNAL_SUCCESS, pal_signal_wait(&signal, (1 << 1), &received_signals, 0, 0, 100));
	EXPECT_EQ((1 << 1), received_signals);
	EXPECT_EQ(signal.signals, (1 << 1));

	pal_signal_destroy(&signal);
}

TEST(pal_os_signal, WaitFiniteTimeoutSuccessAfterWait)
{
	pal_signal_t signal			  = {0};
	size_t		 received_signals = 0;
	pal_signal_create(&signal);

	// Create a thread to set the signal after a delay
	std::thread signal_setter(
		[&]()
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			pal_signal_set(&signal, (1 << 2), 0);
		});

	// Wait for the signal with a finite timeout
	EXPECT_EQ(PAL_SIGNAL_SUCCESS, pal_signal_wait(&signal, (1 << 2), &received_signals, 0, 0, 200));
	EXPECT_EQ((1 << 2), received_signals);
	EXPECT_EQ(signal.signals, (1 << 2));

	// Join the thread and clean up
	signal_setter.join();
	pal_signal_destroy(&signal);
}

TEST(pal_os_signal, WaitFiniteTimeoutFailure)
{
	pal_signal_t signal			  = {0};
	size_t		 received_signals = 0;
	pal_signal_create(&signal);

	// Wait for a signal that is never set, with a finite timeout
	EXPECT_EQ(PAL_SIGNAL_TIMEOUT, pal_signal_wait(&signal, (1 << 3), &received_signals, 0, 0, 100));
	EXPECT_EQ(0, received_signals);

	pal_signal_destroy(&signal);
}
