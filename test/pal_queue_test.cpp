#include <gtest/gtest.h>
#include <pthread.h>

#include "pal_os/common.h"
#include "pal_os/queue.h"
#include "queue_priv.h"

TEST(pal_os_queue, createQueueSuccess)
{
	pal_queue_t *queue	   = NULL;
	int			 mutexType = 0;
	EXPECT_EQ(0, pal_queue_create(&queue, sizeof(int), 10));
	EXPECT_NE(nullptr, queue);
	EXPECT_EQ(10, queue->max_items);
	EXPECT_EQ(0, queue->head);
	EXPECT_EQ(0, queue->tail);
	EXPECT_EQ(sizeof(int), queue->item_size);
	EXPECT_NE(nullptr, queue->data);
	EXPECT_EQ(10, pal_queue_get_free_slots(queue));
	EXPECT_EQ(0, pal_queue_get_items(queue, 0));
	pal_queue_destroy(&queue);
	EXPECT_EQ(nullptr, queue);
}

TEST(pal_os_queue, createQueueFailureNullPointer)
{
	pal_queue_t *queue = NULL;
	EXPECT_EQ(-1, pal_queue_create(nullptr, sizeof(int), 10));
	EXPECT_EQ(nullptr, queue);
}

TEST(pal_os_queue, createQueueFailureNoItemSize)
{
	pal_queue_t *queue = NULL;
	EXPECT_EQ(-1, pal_queue_create(&queue, 0, 10));
	EXPECT_EQ(nullptr, queue);
}

TEST(pal_os_queue, createQueueFailureNoMaxItems)
{
	pal_queue_t *queue = NULL;
	EXPECT_EQ(-1, pal_queue_create(&queue, sizeof(int), 0));
	EXPECT_EQ(nullptr, queue);
}

TEST(pal_os_queue, DequeAlreadyFilledQueueNoTimeout)
{
	pal_queue_t *queue		   = NULL;
	int			 itemToAdd	   = 10;
	int			 retrievedItem = 0;
	EXPECT_EQ(0, pal_queue_create(&queue, sizeof(int), 10));
	EXPECT_NE(nullptr, queue);
	EXPECT_EQ(0, pal_queue_enqueue(queue, &itemToAdd, PAL_OS_NO_TIMEOUT, 0));
	EXPECT_EQ(1, pal_queue_get_items(queue, 0));
	EXPECT_EQ(9, pal_queue_get_free_slots(queue));
	EXPECT_EQ(0, pal_queue_dequeue(queue, &retrievedItem, PAL_OS_NO_TIMEOUT, 0));
	pal_queue_destroy(&queue);
	EXPECT_EQ(nullptr, queue);
}

TEST(pal_os_queue, DequeAlreadyFilledQueueWithTimeout)
{
	pal_queue_t *queue		   = NULL;
	time_t		 start_time	   = 0;
	time_t		 stop_time	   = 0;
	int			 itemToAdd	   = 10;
	int			 retrievedItem = 0;
	EXPECT_EQ(0, pal_queue_create(&queue, sizeof(int), 10));
	EXPECT_NE(nullptr, queue);
	start_time = time(NULL);
	EXPECT_EQ(0, pal_queue_enqueue(queue, &itemToAdd, 2000, 0));
	stop_time = time(NULL);
	EXPECT_EQ(0, stop_time - start_time);
	EXPECT_EQ(1, pal_queue_get_items(queue, 0));
	EXPECT_EQ(9, pal_queue_get_free_slots(queue));
	start_time = time(NULL);
	EXPECT_EQ(0, pal_queue_dequeue(queue, &retrievedItem, 2000, 0));
	stop_time = time(NULL);
	EXPECT_EQ(0, stop_time - start_time);
	pal_queue_destroy(&queue);
	EXPECT_EQ(nullptr, queue);
}

TEST(pal_os_queue, DequeFailedNoitemsNoTimeout)
{
	pal_queue_t *queue		   = NULL;
	int			 itemToAdd	   = 10;
	int			 retrievedItem = 0;
	EXPECT_EQ(0, pal_queue_create(&queue, sizeof(int), 10));
	EXPECT_NE(nullptr, queue);
	EXPECT_EQ(0, pal_queue_get_items(queue, 0));
	EXPECT_EQ(10, pal_queue_get_free_slots(queue));
	EXPECT_EQ(-1, pal_queue_dequeue(queue, &retrievedItem, PAL_OS_NO_TIMEOUT, 0));
	pal_queue_destroy(&queue);
	EXPECT_EQ(nullptr, queue);
}

TEST(pal_os_queue, DequeFailedNoitemsWithTimeout)
{
	pal_queue_t *queue		   = NULL;
	int			 itemToAdd	   = 10;
	int			 retrievedItem = 0;
	time_t		 start_time	   = 0;
	time_t		 stop_time	   = 0;
	EXPECT_EQ(0, pal_queue_create(&queue, sizeof(int), 10));
	EXPECT_NE(nullptr, queue);
	EXPECT_EQ(0, pal_queue_get_items(queue, 0));
	EXPECT_EQ(10, pal_queue_get_free_slots(queue));
	start_time = time(NULL);
	EXPECT_EQ(-1, pal_queue_dequeue(queue, &retrievedItem, 2000, 0));
	stop_time = time(NULL);
	EXPECT_EQ(2, stop_time - start_time);
	pal_queue_destroy(&queue);
	EXPECT_EQ(nullptr, queue);
}

TEST(pal_os_queue, DequeueInOrder)
{
	pal_queue_t *queue		   = NULL;
	int			 itemToAdd	   = 1;
	int			 retrievedItem = 0;
	EXPECT_EQ(0, pal_queue_create(&queue, sizeof(int), 10));
	EXPECT_NE(nullptr, queue);
	EXPECT_EQ(0, pal_queue_enqueue(queue, &itemToAdd, PAL_OS_NO_TIMEOUT, 0));
	itemToAdd++;
	EXPECT_EQ(0, pal_queue_enqueue(queue, &itemToAdd, PAL_OS_NO_TIMEOUT, 0));
	itemToAdd++;
	EXPECT_EQ(0, pal_queue_enqueue(queue, &itemToAdd, PAL_OS_NO_TIMEOUT, 0));
	itemToAdd++;
	EXPECT_EQ(0, pal_queue_enqueue(queue, &itemToAdd, PAL_OS_NO_TIMEOUT, 0));
	itemToAdd++;
	EXPECT_EQ(0, pal_queue_enqueue(queue, &itemToAdd, PAL_OS_NO_TIMEOUT, 0));
	itemToAdd++;
	EXPECT_EQ(0, pal_queue_enqueue(queue, &itemToAdd, PAL_OS_NO_TIMEOUT, 0));
	itemToAdd++;
	EXPECT_EQ(0, pal_queue_dequeue(queue, &retrievedItem, PAL_OS_NO_TIMEOUT, 0));
	EXPECT_EQ(1, retrievedItem);
	EXPECT_EQ(0, pal_queue_dequeue(queue, &retrievedItem, PAL_OS_NO_TIMEOUT, 0));
	EXPECT_EQ(2, retrievedItem);
	EXPECT_EQ(0, pal_queue_dequeue(queue, &retrievedItem, PAL_OS_NO_TIMEOUT, 0));
	EXPECT_EQ(3, retrievedItem);
	EXPECT_EQ(0, pal_queue_dequeue(queue, &retrievedItem, PAL_OS_NO_TIMEOUT, 0));
	EXPECT_EQ(4, retrievedItem);
	EXPECT_EQ(0, pal_queue_dequeue(queue, &retrievedItem, PAL_OS_NO_TIMEOUT, 0));
	EXPECT_EQ(5, retrievedItem);
	EXPECT_EQ(0, pal_queue_enqueue(queue, &itemToAdd, PAL_OS_NO_TIMEOUT, 0));
	itemToAdd++;
	EXPECT_EQ(0, pal_queue_enqueue(queue, &itemToAdd, PAL_OS_NO_TIMEOUT, 0));
	itemToAdd++;
	EXPECT_EQ(0, pal_queue_enqueue(queue, &itemToAdd, PAL_OS_NO_TIMEOUT, 0));
	itemToAdd++;
	EXPECT_EQ(0, pal_queue_enqueue(queue, &itemToAdd, PAL_OS_NO_TIMEOUT, 0));
	itemToAdd++;
	EXPECT_EQ(0, pal_queue_enqueue(queue, &itemToAdd, PAL_OS_NO_TIMEOUT, 0));
	itemToAdd++;
	EXPECT_EQ(0, pal_queue_enqueue(queue, &itemToAdd, PAL_OS_NO_TIMEOUT, 0));
	itemToAdd++;
	EXPECT_EQ(0, pal_queue_dequeue(queue, &retrievedItem, PAL_OS_NO_TIMEOUT, 0));
	EXPECT_EQ(6, retrievedItem);
	EXPECT_EQ(0, pal_queue_dequeue(queue, &retrievedItem, PAL_OS_NO_TIMEOUT, 0));
	EXPECT_EQ(7, retrievedItem);
	EXPECT_EQ(0, pal_queue_dequeue(queue, &retrievedItem, PAL_OS_NO_TIMEOUT, 0));
	EXPECT_EQ(8, retrievedItem);
	EXPECT_EQ(0, pal_queue_dequeue(queue, &retrievedItem, PAL_OS_NO_TIMEOUT, 0));
	EXPECT_EQ(9, retrievedItem);
	EXPECT_EQ(0, pal_queue_dequeue(queue, &retrievedItem, PAL_OS_NO_TIMEOUT, 0));
	EXPECT_EQ(10, retrievedItem);
	EXPECT_EQ(0, pal_queue_dequeue(queue, &retrievedItem, PAL_OS_NO_TIMEOUT, 0));
	EXPECT_EQ(11, retrievedItem);
	EXPECT_EQ(0, pal_queue_dequeue(queue, &retrievedItem, PAL_OS_NO_TIMEOUT, 0));
	EXPECT_EQ(12, retrievedItem);
	pal_queue_destroy(&queue);
	EXPECT_EQ(nullptr, queue);
}

TEST(pal_os_queue, EnqueFailFilledQueueNoTimeout)
{
	pal_queue_t *queue	   = NULL;
	int			 itemToAdd = 1;
	EXPECT_EQ(0, pal_queue_create(&queue, sizeof(int), 10));
	EXPECT_NE(nullptr, queue);
	for (int i = 0; i < 10; i++)
	{
		EXPECT_EQ(0, pal_queue_enqueue(queue, &itemToAdd, PAL_OS_NO_TIMEOUT, 0));
		itemToAdd++;
	}
	EXPECT_EQ(-1, pal_queue_enqueue(queue, &itemToAdd, PAL_OS_NO_TIMEOUT, 0));
	pal_queue_destroy(&queue);
	EXPECT_EQ(nullptr, queue);
}

TEST(pal_os_queue, EnqueFailFilledQueueWithTimeout)
{
	pal_queue_t *queue		= NULL;
	int			 itemToAdd	= 1;
	time_t		 start_time = 0;
	time_t		 stop_time	= 0;
	EXPECT_EQ(0, pal_queue_create(&queue, sizeof(int), 10));
	EXPECT_NE(nullptr, queue);
	for (int i = 0; i < 10; i++)
	{
		EXPECT_EQ(0, pal_queue_enqueue(queue, &itemToAdd, PAL_OS_NO_TIMEOUT, 0));
		itemToAdd++;
	}
	start_time = time(NULL);
	EXPECT_EQ(-1, pal_queue_enqueue(queue, &itemToAdd, 2000, 0));
	stop_time = time(NULL);
	EXPECT_EQ(2, stop_time - start_time);
	pal_queue_destroy(&queue);
	EXPECT_EQ(nullptr, queue);
}

TEST(pal_os_queue, ResetQueue)
{
	pal_queue_t *queue		   = NULL;
	int			 itemToAdd	   = 1;
	int			 retrievedItem = 0;
	EXPECT_EQ(0, pal_queue_create(&queue, sizeof(int), 10));
	EXPECT_NE(nullptr, queue);
	for (int i = 0; i < 6; i++)
	{
		EXPECT_EQ(0, pal_queue_enqueue(queue, &itemToAdd, PAL_OS_NO_TIMEOUT, 0));
		itemToAdd++;
	}
	EXPECT_EQ(6, pal_queue_get_items(queue, 0));
	EXPECT_EQ(4, pal_queue_get_free_slots(queue));
	EXPECT_EQ(6, queue->tail);
	EXPECT_EQ(0, queue->head);
	EXPECT_EQ(0, pal_queue_dequeue(queue, &retrievedItem, PAL_OS_NO_TIMEOUT, 0));
	EXPECT_EQ(6, queue->tail);
	EXPECT_EQ(1, queue->head);
	EXPECT_EQ(5, pal_queue_get_items(queue, 0));
	EXPECT_EQ(5, pal_queue_get_free_slots(queue));
	pal_queue_reset(queue);
	EXPECT_EQ(0, queue->tail);
	EXPECT_EQ(0, queue->head);
	EXPECT_EQ(0, pal_queue_get_items(queue, 0));
	EXPECT_EQ(10, pal_queue_get_free_slots(queue));
	EXPECT_EQ(-1, pal_queue_dequeue(queue, &retrievedItem, PAL_OS_NO_TIMEOUT, 0));
	pal_queue_destroy(&queue);
	EXPECT_EQ(nullptr, queue);
}