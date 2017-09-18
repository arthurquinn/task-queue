#include <gtest/gtest.h>
#include "task_manager.h"

namespace {

// The fixture for testing class Foo.
class TaskManagerTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  TaskManager task_manager;

  TaskManagerTest() : task_manager(100, "queue_item_tmp") {
    // You can do set-up work for each test here.

  }

  virtual ~TaskManagerTest() {
    // You can do clean-up work that doesn't throw exceptions here.
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
    system("rm -rf queue_item_tmp");
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

// Tests that tasks are successfully added to the task manager
TEST_F(TaskManagerTest, PushTaskTest) {
  task_manager.push_task(1, "command 1");
  task_manager.push_task(2, "command 2");
  task_manager.push_task(3, "command 3");
  task_manager.push_task(4, "command 4");
  task_manager.push_task(5, "command 5");
}

TEST_F(TaskManagerTest, LoadTaskTest) {

}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}