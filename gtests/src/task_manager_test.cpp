#include <gtest/gtest.h>
#include "task_manager.h"

namespace {

// The fixture for testing class Foo.
class TaskManagerTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  TaskManager task_manager;

  TaskManagerTest() : task_manager(20, "task_item_tmp") {
    // You can do set-up work for each test here.

    task_manager.push_task(100, "sleep 10 && ls -l");
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
  task_manager.stage_task();

  system("sleep 15");
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}