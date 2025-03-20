#include <filesystem>
#include "block/block_instance.h"
#include "gtest/gtest.h"
#include "public/block_library.h"

TEST(ModelLayerTests, INIT_FILE_EXISTS) {
  std::filesystem::path path(model::BlockLibrary::block_folder_pathname);
  path /= model::BlockLibrary::block_initfile_pathname;

  EXPECT_TRUE(std::filesystem::exists(path));
}

TEST(ModelLayerTests, BlockLibrary) {
  auto& lib = model::BlockLibrary::instance();
  lib.initialize();
}

TEST(ModelLayerTests, DummyBlockInstances) {
  auto& lib = model::BlockLibrary::instance();
  // get some random definitons
  // TODO: complete this test
}
