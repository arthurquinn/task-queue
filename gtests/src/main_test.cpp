#include <gtest/gtest.h>
#include <climits>
#include "queue.h"
#include "queue_item.h"

namespace {

struct sample_data {
  int x;
  int y;
  int z;
};


#define QUEUE_LEN 10

// The fixture for testing class Foo.
class QueueTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  Queue * test_queue;
  sample_data * sd;
  QueueItem * items[QUEUE_LEN];

  QueueTest() {
    // You can do set-up work for each test here.

    system("mkdir queue_item_tmp");

    // set up queue
    test_queue = new Queue(QUEUE_LEN, "queue_item_tmp");

    // set up sample datas
    sd = new sample_data[QUEUE_LEN];
    for (int i = 0; i < QUEUE_LEN; i++) {
      sd[i].x = i * 10;
      sd[i].y = i * 10;
      sd[i].z = i * 10;
    }

    // set up queue items
    for (int i = 0; i < QUEUE_LEN; i++) {
      items[i] = new QueueItem(i, &sd[i], sizeof(struct sample_data));
    }
  }

  virtual ~QueueTest() {
    // You can do clean-up work that doesn't throw exceptions here.
    system("rm -rf queue_item_tmp");
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
    delete test_queue;
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

// simply runs through and make sure that the enqueues are successful
TEST_F(QueueTest, EnqueueTest) {
  for (int i = 0; i < QUEUE_LEN; i++) {
    test_queue->enqueue(*items[i]);
  }
}

// test enqueueing more items than max length of queue
TEST_F(QueueTest, EnqueueOverflowTest) {
  for (int i = 0; i < QUEUE_LEN; i++) {
    test_queue->enqueue(*items[i]);
  }

  struct sample_data data;
  QueueItem item(0, &data, sizeof(struct sample_data));

  EXPECT_THROW(test_queue->enqueue(item), std::exception);
}

// test peeking an empty queue
TEST_F(QueueTest, EnqueueEmptyPeekTest) {
  EXPECT_THROW(test_queue->peek(), std::exception);
}

// test dequeueing an empty queue
TEST_F(QueueTest, DequeueEmptyTest) {
  EXPECT_THROW(test_queue->dequeue(), std::exception);
}

// test the highest priority is on top after enqueueing out of order
TEST_F(QueueTest, PriorityTest) {
  struct sample_data data;
  QueueItem highest(255, &data, sizeof(struct sample_data));
  QueueItem middle(150, &data, sizeof(struct sample_data));
  QueueItem lowest(0, &data, sizeof(struct sample_data));

  test_queue->enqueue(middle);
  test_queue->enqueue(highest);
  test_queue->enqueue(lowest);

  QueueItem top = test_queue->peek();
  EXPECT_EQ(255, top.priority());
}

// ensure queue loads from binary files properly
TEST_F(QueueTest, LoadTest) {
  Queue queue(10, "gtest_queue_items");
  EXPECT_EQ(10, queue.size());
}

// ensure queue loads from queue_item files correctly
TEST_F(QueueTest, LoadCorrectDataTest) {
  Queue queue(10, "gtest_queue_items");

  QueueItem top = queue.peek();
  struct sample_data * data = (struct sample_data *)top.data();

  EXPECT_EQ(90, data->x);
  EXPECT_EQ(99, data->y);
  EXPECT_EQ(108, data->z);
}

// ensure invalid queue items are not read into the queue
TEST_F(QueueTest, LoadIncorrectDataTest) {
  Queue queue(10, "bad_queue_items");
  EXPECT_EQ(0, queue.size());
}

TEST_F(QueueTest, QueueSameItemTwiceTest) {
  struct sample_data data;
  QueueItem item(100, &data, sizeof(struct sample_data));

  test_queue->enqueue(item);
  EXPECT_THROW(test_queue->enqueue(item), std::exception);
}

TEST_F(QueueTest, LoadInSameOrderAsEnqueuedTest) {
  Queue queue(10, "tmp");
  for (int i = 0; i < 10; i++) {
    int priority = i < 5 ? 0 : 1;
    QueueItem qi(priority, &sd[i], sizeof(struct sample_data));
    queue.enqueue(qi);
  }

  Queue loaded_queue(10, "tmp");
  int push_index = -1;
  int priority = INT_MAX;
  for (int i = 0; i < 10; i++) {
    QueueItem top = loaded_queue.peek();
    int item_priority = static_cast<int>(top.priority());
    int item_push_index = static_cast<int>(top.push_index());
    EXPECT_LE(item_priority, priority);
    if (item_priority != priority) {
      push_index = -1;
    }
    EXPECT_GT(item_push_index, push_index);
    priority = item_priority;
    push_index = item_push_index;
    loaded_queue.dequeue();
  }

  system("rm -rf tmp");
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}