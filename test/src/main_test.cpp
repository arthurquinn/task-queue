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
    system("rm -rf tmp");
    system("rm -rf queue_item_tmp");
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
    test_queue->enqueue(items[i]);
  }
}

// test enqueueing more items than max length of queue
TEST_F(QueueTest, EnqueueOverflowTest) {
  for (int i = 0; i < QUEUE_LEN; i++) {
    test_queue->enqueue(items[i]);
  }

  struct sample_data data;
  QueueItem item(0, &data, sizeof(struct sample_data));

  EXPECT_THROW(test_queue->enqueue(&item), std::exception);
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

  test_queue->enqueue(&middle);
  test_queue->enqueue(&highest);
  test_queue->enqueue(&lowest);

  const QueueItem* top = test_queue->peek();
  EXPECT_EQ(255, top->priority());
}

// ensure queue loads from binary files properly
TEST_F(QueueTest, LoadTest) {
  Queue queue(10, "gtest_queue_items");
  EXPECT_EQ(10, queue.size());
}

// ensure queue loads from queue_item files correctly
TEST_F(QueueTest, LoadCorrectDataTest) {
  Queue queue(10, "gtest_queue_items");

  const QueueItem* top = queue.peek();
  struct sample_data * data = (struct sample_data *)top->data();

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

  test_queue->enqueue(&item);
  EXPECT_THROW(test_queue->enqueue(&item), std::exception);
}

TEST_F(QueueTest, LoadInSameOrderAsEnqueuedTest) {
  int QUEUE_SIZE = 12;

  Queue queue(QUEUE_SIZE, "tmp");
  QueueItem* queue_items[QUEUE_SIZE];

  for (int i = 0; i < QUEUE_SIZE - 2; i++) {
    int priority = i < 5 ? 0 : 1;
    queue_items[i] = new QueueItem(priority, &sd[i], sizeof(struct sample_data));
    queue.enqueue(queue_items[i]);
    std::cout << "enqueueing: priority = " << static_cast<int>(queue_items[i]->priority()) << ", push_index = " << queue_items[i]->push_index() << ", data = " << sd[i].x << " " << sd[i].y << " " << sd[i].z << std::endl;
  }

  std::cout << "DONE ENQUEUEING" << std::endl;

  struct sample_data extra1;
  extra1.x = -1;
  extra1.y = -1;
  extra1.z = -1;

  struct sample_data extra2;
  extra2.x = -2;
  extra2.y = -2;
  extra2.z = -2;

  queue_items[QUEUE_SIZE - 2] = new QueueItem(1, &extra1, sizeof(struct sample_data));
  queue_items[QUEUE_SIZE - 1] = new QueueItem(0, &extra2, sizeof(struct sample_data));

  Queue loaded_queue(QUEUE_SIZE, "tmp");
  std::cout << "LOADED FROM DISK" << std::endl;

  loaded_queue.enqueue(queue_items[QUEUE_SIZE - 2]);
  loaded_queue.enqueue(queue_items[QUEUE_SIZE - 1]);

  std::cout << "ENQUEUEING EXTRAS..." << std::endl;
  std::cout << "enqueueing: priority = " << static_cast<int>(queue_items[QUEUE_SIZE - 2]->priority()) << ", push_index = " << queue_items[QUEUE_SIZE - 2]->push_index() << ", data = " << extra1.x << " " << extra1.y << " " << extra1.z << std::endl;
  std::cout << "enqueueing: priority = " << static_cast<int>(queue_items[QUEUE_SIZE - 1]->priority()) << ", push_index = " << queue_items[QUEUE_SIZE - 1]->push_index() << ", data = " << extra2.x << " " << extra2.y << " " << extra2.z << std::endl;

  std::cout << "VALIDATING ORDER..." << std::endl;
  int push_index = 0;
  int priority = INT_MAX;
  for (int i = 0; i < QUEUE_SIZE; i++) {
    const QueueItem* top = loaded_queue.peek();
    const unsigned char item_priority = top->priority();
    const unsigned long item_push_index = top->push_index();

    const struct sample_data* sdptr = static_cast<const struct sample_data*>(top->data());
    std::cout << "loaded: " << "priority = " << static_cast<unsigned int>(item_priority) << ", push_index = " << item_push_index << ", data = " << sdptr->x << " " << sdptr->y << " " << sdptr->z << std::endl;

    EXPECT_LE(item_priority, priority);
    if (item_priority != priority) {
      push_index = 0;
    }
    EXPECT_GE(item_push_index, push_index);
    priority = item_priority;
    push_index = item_push_index;
    loaded_queue.dequeue();
  }

  std::cout << "DONE!" << std::endl;
  system("rm -rf tmp");
}

TEST_F(QueueTest, QueuePushCursorCorrectAfterLoad) {
  
  // Start with full queue
  Queue this_queue(QUEUE_LEN, "tmp");
  QueueItem* queue_items[QUEUE_LEN];
  for (int i = 0; i < QUEUE_LEN; i++) {
    queue_items[i] = new QueueItem(0, &sd[i], sizeof(struct sample_data));
    this_queue.enqueue(queue_items[i]);
    std::cout << "enqueueing: priority = " << static_cast<int>(queue_items[i]->priority()) << ", push_index = " << queue_items[i]->push_index() << ", data = " << sd[i].x << " " << sd[i].y << " " << sd[i].z << std::endl;
  }

  // Queue push cursor should be QUEUE_LEN
  std::cout << "dequeueing top 2..." << std::endl;
  QueueItem* tmp;
  EXPECT_EQ(this_queue.current_push_cursor(), QUEUE_LEN);
  tmp = this_queue.dequeue();
  tmp->remove();
  std::cout << "dequeueing: priority = " << static_cast<int>(tmp->priority()) << ", push_index = " << tmp->push_index() << std::endl;
  tmp = this_queue.dequeue();
  tmp->remove();
  std::cout << "dequeueing: priority = " << static_cast<int>(tmp->priority()) << ", push_index = " << tmp->push_index() << std::endl;

  // Queue push cursor should still be QUEUE_LEN
  EXPECT_EQ(this_queue.current_push_cursor(), QUEUE_LEN);
  std::cout << "PUSH CURSOR BEFORE LOAD: " << this_queue.current_push_cursor() << std::endl;

  Queue loaded_queue(QUEUE_LEN, "tmp");

  // After load push cursor should still be QUEUE_LEN even though we have QUEUE_LEN - 2 items in the queue
  EXPECT_EQ(loaded_queue.current_push_cursor(), QUEUE_LEN);

  while (loaded_queue.size() > 0) {
    tmp = loaded_queue.dequeue();
    std::cout << "final queue: priority = " << static_cast<int>(tmp->priority()) << ", push_index = " << tmp->push_index() << std::endl;
  }

  std::cout << "PUSH CURSOR AFTER LOAD: " << loaded_queue.current_push_cursor() << std::endl;

  system("rm -rf tmp");
}

TEST_F(QueueTest, QueueItemNullPtrGiven) {

  Queue this_queue(1, "tmp");

  // Enqueue a queue item with a null ptr to raw data
  QueueItem* qi = new QueueItem(255, NULL, 0);
  this_queue.enqueue(qi);

  // Attempt to load this queue item
  Queue loaded_queue(1, "tmp");
  QueueItem* loaded_qi = loaded_queue.dequeue();

  // Ensure queue item was loaded properly
  EXPECT_EQ(loaded_qi->priority(), 255);

  system("rm -rf tmp");
}

TEST_F(QueueTest, QueueItemDeleteRawDataTest) {
  struct sample_data* this_sd = new struct sample_data();
  this_sd->x = 10;
  this_sd->y = 20;
  this_sd->z = 30;

  // store this_sd in queue
  QueueItem* qi = new QueueItem(0, this_sd, sizeof(struct sample_data));
  test_queue->enqueue(qi);

  // delete our copy of this_sd
  delete this_sd;

  // get the queue item's copy of sample data and ensure values are correct
  const struct sample_data* their_sd = reinterpret_cast<const struct sample_data*>(qi->data());
  EXPECT_EQ(their_sd->x, 10);
  EXPECT_EQ(their_sd->y, 20);
  EXPECT_EQ(their_sd->z, 30);
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}