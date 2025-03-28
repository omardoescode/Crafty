#include <algorithm>
#include <bitset>
#include <numeric>
#include <utility>
#include <vector>
#include "events/event_dispatcher.h"
#include "gtest/gtest.h"
#include "string"

// TEST(CommonLayerTests, InitializeEventDispatcher) {
//   auto& dispatcher = common::EventDispatcher::instance();
//   int before = 1;
//   int after = 10;
//   int res = before;
//   dispatcher.subscribe<EventTest>(
//       [&](std::shared_ptr<EventTest> evt) { res = evt->val; });
//
//   EXPECT_EQ(res, before);
//   dispatcher.publish(std::make_shared<EventTest>(after));
//   EXPECT_EQ(res, after);
// }

class EventDispatcherTest : public testing::Test {
public:
  struct SimpleEvent : public common::EventBase {
    int val;
    SimpleEvent(int value) : val(value) {}
  };
  struct DerivedEvent : public SimpleEvent {
    std::string msg;
    DerivedEvent(int val, const std::string& msg)
        : SimpleEvent(val), msg(msg) {}
  };

  void SetUp() override { dispatcher.clear_all(); }
  void TearDown() override {}

  common::EventDispatcher& dispatcher = common::EventDispatcher::instance();
};

TEST_F(EventDispatcherTest, BasicTest) {
  constexpr int after = 42;
  int result = 0;
  dispatcher.subscribe<SimpleEvent>(
      [&](std::shared_ptr<SimpleEvent> evt) { result = evt->val; });

  dispatcher.publish(std::make_shared<SimpleEvent>(after));
  EXPECT_EQ(result, after);
}

TEST_F(EventDispatcherTest, NoSubscribersTest) {
  EXPECT_NO_THROW({ dispatcher.publish(std::make_shared<SimpleEvent>(999)); });
}
TEST_F(EventDispatcherTest, EventInheritance) {
  constexpr int after = 42;
  const std::string after_s = "world";
  int result = 0;
  std::string msg = "";
  dispatcher.subscribe<DerivedEvent>([&](std::shared_ptr<DerivedEvent> evt) {
    result = evt->val;
    msg = evt->msg;
  });

  dispatcher.publish(std::make_shared<DerivedEvent>(after, after_s));
  EXPECT_EQ(result, after);
  EXPECT_EQ(msg, after_s);
}
TEST_F(EventDispatcherTest, ThreadSafetyMultiplePublish) {
  std::atomic<long long> result{0};

  constexpr int sz = 10000;
  std::array<int, sz> arr;
  std::fill(begin(arr), end(arr), 10);
  int sum = std::accumulate(begin(arr), end(arr), 0);

  std::vector<std::thread> threads;
  dispatcher.subscribe<SimpleEvent>([&](std::shared_ptr<SimpleEvent> evt) {
    int idx = (evt->val);
    result += arr[idx];
  });

  for (int i = 0; i < sz; i++) {
    threads.emplace_back(
        [&, i]() { dispatcher.publish(std::make_shared<SimpleEvent>(i)); });
  }

  // Wait for all threads
  for (auto& t : threads) {
    t.join();
  }

  // Result should be predictable
  EXPECT_EQ(result, sum);
}

TEST_F(EventDispatcherTest, ThreadSafetyMultipleSubscribe) {
  std::atomic<long long> result{0};

  constexpr int sub_count = 10000;
  constexpr int sz = 10000;
  std::array<int, sz> arr;
  std::fill(begin(arr), end(arr), 10);

  std::bitset<sz> added;
  added.reset();
  int sum = std::accumulate(begin(arr), end(arr), 0);

  std::vector<std::thread> threads;
  for (int i = 0; i < sub_count; i++)
    threads.emplace_back([&]() {
      dispatcher.subscribe<SimpleEvent>([&](std::shared_ptr<SimpleEvent> evt) {
        int idx = (evt->val);
        result += arr[idx];
      });
    });

  // Wait for all suscribing threads
  for (auto& t : threads) {
    t.join();
  }
  threads.clear();
  for (int i = 0; i < sz; i++) {
    threads.emplace_back(
        [&, i]() { dispatcher.publish(std::make_shared<SimpleEvent>(i)); });
  }

  // Wait for all publishing threads
  for (auto& t : threads) {
    t.join();
  }

  // Result should be predictable
  EXPECT_EQ(result, sum * sub_count);
}
