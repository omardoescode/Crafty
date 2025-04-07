#include <stdexcept>
#include <thread>
#include "action_deferrer.h"
#include "gtest/gtest.h"

class ActionDeferrerTest : public testing::Test {
public:
  void SetUp() override {}
  void TearDown() override {}
  ui::ActionDeferrer& deferrer = ui::ActionDeferrer::instance();
};

TEST_F(ActionDeferrerTest, BasicTest) {
  auto& deferrer = ui::ActionDeferrer::instance();
  int modified = 0;

  int counter = 100;
  for (int i = 0; i < counter; i++) {
    deferrer.defer([&]() { modified++; });
  }

  deferrer.flush();
  EXPECT_EQ(modified, counter);
}

TEST_F(ActionDeferrerTest, MultiplePriority) {
  int result = 42;
  deferrer.defer([&]() { result = 20; }, 1);
  deferrer.defer([&]() { result = 23; }, 2);

  deferrer.flush();
  // Assignment to 20 will be done last since it has a lower priority
  EXPECT_EQ(result, 20);
}

TEST_F(ActionDeferrerTest, ThreadSafetyMultipleDeferring) {
  int modified = 0;

  int counter = 100;
  std::vector<std::thread> threads;
  for (int i = 0; i < counter; i++) {
    threads.emplace_back([&]() { deferrer.defer([&]() { modified++; }); });
  }

  for (auto& t : threads) t.join();

  deferrer.flush();
  EXPECT_EQ(modified, counter);
}

TEST_F(ActionDeferrerTest, DebugTagPrinting) {
  int modified = 0;

  int counter = 100;
  const int zero = 0;
  deferrer.defer([&]() { throw std::runtime_error("Hello"); });

  // negate stdout
  std::streambuf* originalStdout = std::cout.rdbuf();
  std::cout.rdbuf(nullptr);
  EXPECT_NO_THROW(deferrer.flush());

  // restore stdout
  std::cout.rdbuf(originalStdout);
}
