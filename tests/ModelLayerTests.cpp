#include <filesystem>
#include "block/BlockLibrary.h"
#include "gtest/gtest.h"

TEST(ModelLayerTests, INIT_FILE_EXISTS) {
  std::filesystem::path path(model::BlockLibrary::block_folder_pathname);
  path /= model::BlockLibrary::block_initfile_pathname;

  EXPECT_TRUE(std::filesystem::exists(path));
}

TEST(ModelLayerTests, BlockLibrary) {
  auto& lib = model::BlockLibrary::instance();
  lib.initialize();
}
