#include <gtest/gtest.h>
#include "task_manager.h"

#include <unistd.h>

namespace {

// The fixture for testing class Foo.
class TaskManagerTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  TaskManager task_manager;

  TaskManagerTest() : task_manager(20, "task_item_tmp") {
    // You can do set-up work for each test here.

    task_manager.push_task(100, "./sample_udp_message/udp_message");
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
    system("rm -rf task_item_tmp");
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(TaskManagerTest, RunSingleTaskTest) {

  while (1) {
    task_manager.run();
    sleep(1);
    task_manager.push_task(5, "./sample_udp_message/udp_message");
  }
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}