#include <filesystem>
#include "block/block_library.h"
#include "block/json_block_storage.h"
#include "gtest/gtest.h"
#include "identity/id/id.h"
#include "identity/id/id_generator.h"
#include "identity/id/prefixed_id_generator.h"
#include "identity/id/store.h"
#include "identity/id/string_id.h"
#include "utils/fs.h"

TEST(ModelLayerTests, BlockLibraryInitialization) {
  std::unique_ptr<model::BlockStorage> storage(
      std::make_unique<model::JsonBlockStorage>(
          std::make_unique<model::PrefixedIDGenerator>("test")));
  model::BlockLibrary::Config config;
  config.block_file_path = "./data/blocks.json";
  model::BlockLibrary lib(std::move(storage), config);
}

TEST(IDTest, TestStringID) {
  model::IDPtr id1 = std::make_shared<model::StringID>("ID1");
  model::IDPtr id2 = std::make_shared<model::StringID>("ID2");
  model::IDPtr id1_copy = std::make_shared<model::StringID>("ID1");

  EXPECT_TRUE(id1 < id2);
  EXPECT_FALSE(id1 == id2);
  EXPECT_FALSE(id1 < id1_copy);
  EXPECT_TRUE(id1 == id1_copy);

  EXPECT_TRUE(id1->to_string() == "ID1");
  EXPECT_TRUE(id2->to_string() == "ID2");
}

TEST(IDTest, TestPrefixedIDGeneratorComparison) {
  model::IDGeneratorPtr test_gen =
      std::make_unique<model::PrefixedIDGenerator>("test");
  model::IDGeneratorPtr test2_gen =
      std::make_unique<model::PrefixedIDGenerator>("test2");

  model::IDPtr id1{test_gen->generate_next()};
  model::IDPtr id2{test2_gen->generate_next()};

  EXPECT_FALSE(id1 == id2);
  EXPECT_TRUE(id1 < id2);
  EXPECT_TRUE(*id1 < *id2);
}

TEST(FSTests, NormalTest) {
  std::filesystem::path path_windows("C:/Users/Class1");
  std::filesystem::path path_unix("/home/user/");
  EXPECT_TRUE(construct_path(path_windows, "Potates") ==
              "C:/Users/Class1/Potates");
  EXPECT_TRUE(construct_path(path_unix, "Potates") == "/home/user/Potates");
}

TEST(StoreTests, InitializationTest) {
  struct test_st {
    model::IDPtr id;
    int val;
    test_st(model::IDPtr id, int val) : id(id), val(val) {}
  };
  model::IDGeneratorPtr mgr =
      std::make_unique<model::PrefixedIDGenerator>("test");
  model::Store<test_st> test_store(std::move(mgr));

  model::IDPtr id1 = std::make_shared<model::StringID>("test0");
  EXPECT_FALSE(test_store.has_entity(id1));
  test_store.create_entity(3);
  EXPECT_TRUE(test_store.has_entity(id1));
  auto entity = test_store.get_entity(id1);
  EXPECT_TRUE(entity->val == 3);
}
