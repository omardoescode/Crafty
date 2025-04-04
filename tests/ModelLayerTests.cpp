#include <filesystem>
#include <limits>
#include "block/block_library.h"
#include "gtest/gtest.h"
#include "utils/ID_manager.h"
#include "utils/fs.h"
#include "utils/store.h"

TEST(ModelLayerTests, BlockLibraryInitialization) {
  auto& lib = model::BlockLibrary::instance();
  lib.initialize();
}

TEST(ModelLayerTests, DummyBlockInstances) {
  auto& lib = model::BlockLibrary::instance();
  // get some random definitons
  // TODO: complete this test
}

TEST(IDManagerTests, NormalTest) {
  model::IDManager mgr("test");
  for (int i = 0; i < 10; i++)
    EXPECT_TRUE(mgr.generate_with_counter() == "test" + std::to_string(i));
}
TEST(IDManagerTests, WithStartTest) {
  srand(time(NULL));
  int num = rand();
  model::IDManager mgr("test", num);
  for (int i = 0; i < 10; i++)
    EXPECT_TRUE(mgr.generate_with_counter() ==
                "test" + std::to_string(num + i));
}
TEST(IDManagerTests, WithMaxStartTest) {
  unsigned int mx = std::numeric_limits<unsigned int>::max();
  model::IDManager mgr("test", mx);
  assert(mgr.generate_with_counter() == "test" + std::to_string(mx));
  for (int i = 0; i < 10; i++)
    EXPECT_TRUE(mgr.generate_with_counter() == "test" + std::to_string(i));
}

TEST(FSTests, NormalTest) {
  std::filesystem::path path_windows("C:/Users/Class1");
  std::filesystem::path path_unix("/home/user/");
  EXPECT_TRUE(construct_path(path_windows, "Potates") ==
              "C:/Users/Class1/Potates");
  EXPECT_TRUE(construct_path(path_unix, "Potates") == "/home/user/Potates");
}

struct test_st {
  model::IDManager::IDType id;
  int val;
  test_st(model::IDManager::IDType id, int val) : id(id), val(val) {}
};
TEST(StoreTests, InitializationTest) {
  std::unique_ptr<model::IDManager> mgr =
      std::make_unique<model::IDManager>("test");
  model::Store<test_st> test_store(std::move(mgr));

  EXPECT_FALSE(test_store.has_entity("test0"));
  test_store.create_entity(3);
  EXPECT_TRUE(test_store.has_entity("test0"));
  auto entity = test_store.get_entity("test0");
  EXPECT_TRUE(entity->val == 3);
}
