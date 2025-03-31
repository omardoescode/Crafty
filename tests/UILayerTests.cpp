#include <stdexcept>
#include <thread>
#include "action_deferrer.h"
#include "gtest/gtest.h"

TEST(ActionDeferrerTests, BasicTest) {
  auto& deferrer = ui::ActionDeferrer::instance();
  int modified = 0;

  int counter = 100;
  for (int i = 0; i < counter; i++) {
    deferrer.defer([&]() { modified++; });
  }

  deferrer.flush();
  EXPECT_EQ(modified, counter);
}

TEST(ActionDeferrerTests, MultiplePriority) {
  auto& deferrer = ui::ActionDeferrer::instance();

  int result = 42;
  deferrer.defer([&]() { result = 20; }, 1);
  deferrer.defer([&]() { result = 23; }, 2);

  deferrer.flush();
  // Assignment to 20 will be done last since it has a lower priority
  EXPECT_EQ(result, 20);
}

TEST(ActionDeferrerTests, ThreadSafetyMultipleDeferring) {
  auto& deferrer = ui::ActionDeferrer::instance();
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

TEST(ActionDeferrerTests, DebugTagPrinting) {
  auto& deferrer = ui::ActionDeferrer::instance();
  int modified = 0;

  int counter = 100;
  const int zero = 0;
  deferrer.defer([&]() { throw std::runtime_error("Hello"); });

  EXPECT_NO_THROW(deferrer.flush());
}
