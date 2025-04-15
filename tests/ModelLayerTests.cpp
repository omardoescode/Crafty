#include <filesystem>
#include <memory>
#include "asset.h"
#include "block/block_definition.h"
#include "block/block_instance.h"
#include "block/block_library.h"
#include "block/json_block_storage.h"
#include "block/value.h"
#include "character.h"
#include "gtest/gtest.h"
#include "identity/id.h"
#include "identity/id_generator.h"
#include "identity/identifiable.h"
#include "identity/prefixed_id_generator.h"
#include "identity/store.h"
#include "identity/string_id.h"
#include "project_manager.h"
#include "script.h"
#include "utils/fs.h"

using namespace model;
template <typename Container>
bool contains_id(const Container& container, IDPtr target) {
  return std::find_if(container.begin(), container.end(),
                      [&](const auto& id_w) {
                        auto id = id_w.lock();
                        return id && id == target;
                      }) != container.end();
}

static auto& mgr = ProjectManager::instance();
TEST(ModelLayerTests, BlockLibraryInitialization) {
  std::unique_ptr<BlockStorage> storage(std::make_unique<JsonBlockStorage>(
      std::make_unique<PrefixedIDGenerator>("test")));
  BlockLibrary::Config config;
  config.block_file_path = "./data/blocks.json";
  BlockLibrary lib(std::move(storage), config);
}

class MockConstantID : public ID {
public:
  MockConstantID() = default;
  bool operator==(const ID& rhs) const override { return true; };
  bool operator<(const ID& rhs) const override { return true; }

  std::string to_string() const override { return ""; }
};
class MockConstantIDGenerator : public IDGenerator {
  IDPtr generate_next() { return std::make_shared<MockConstantID>(); }
};

class MockNumberID : public ID {
public:
  MockNumberID(int value) : _value(value) {}
  bool operator==(const ID& rhs) const {
    if (const auto* other = dynamic_cast<const MockNumberID*>(&rhs)) {
      return _value == other->_value;
    }
    return false;
  }
  bool operator<(const ID& rhs) const {
    if (const auto* other = dynamic_cast<const MockNumberID*>(&rhs)) {
      return _value < other->_value;
    }
    return typeid(*this).name() < typeid(rhs).name();
  }

  std::string to_string() const { return ""; }

private:
  int _value;
};

TEST(IDTest, TestStringID) {
  IDPtr id1 = std::make_shared<StringID>("ID1");
  IDPtr id2 = std::make_shared<StringID>("ID2");
  IDPtr id1_copy = std::make_shared<StringID>("ID1");

  EXPECT_TRUE(id1 < id2);
  EXPECT_FALSE(id1 == id2);
  EXPECT_FALSE(id1 < id1_copy);
  EXPECT_TRUE(id1 == id1_copy);

  EXPECT_TRUE(id1->to_string() == "ID1");
  EXPECT_TRUE(id2->to_string() == "ID2");
}

TEST(IDTest, TestPrefixedIDGeneratorComparison) {
  IDGeneratorPtr test_gen = std::make_unique<PrefixedIDGenerator>("test");
  IDGeneratorPtr test2_gen = std::make_unique<PrefixedIDGenerator>("test2");

  IDPtr id1{test_gen->generate_next()};
  IDPtr id2{test2_gen->generate_next()};

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
  struct test_st : public Identifiable {
    int val;
    test_st(IDPtr id, int val) : Identifiable(id), val(val) {}
  };
  IDGeneratorPtr mgr = std::make_unique<MockConstantIDGenerator>();
  Store<test_st> test_store(std::move(mgr));

  auto entity = test_store.create_entity(3);
  EXPECT_TRUE(test_store.has_entity(entity->id()));
  EXPECT_TRUE(entity->val == 3);
}

class EntityTests : public testing::Test {
public:
  std::unique_ptr<IDGenerator> mock_gen =
      std::make_unique<PrefixedIDGenerator>("mock");
};
TEST_F(EntityTests, BasicCharacterTest) {
  auto chr =
      std::make_shared<Character>(mock_gen->generate_next(), 100, 100, 100);
  auto scr = std::make_shared<Script>(mock_gen->generate_next(), chr, 0, 0);

  chr->add_script(scr->id());
  EXPECT_TRUE(chr->has_scripts());
  EXPECT_TRUE(contains_id(chr->scripts(), scr->id()));

  chr->remove_script(scr->id());
  EXPECT_FALSE(chr->has_scripts());
  EXPECT_FALSE(contains_id(chr->scripts(), scr->id()));

  auto sprite = std::make_shared<Asset>(mock_gen->generate_next(), "");

  chr->add_sprite(sprite->id());
  EXPECT_TRUE(chr->has_sprites());
  EXPECT_TRUE(contains_id(chr->sprites(), sprite->id()));

  chr->remove_sprite(sprite->id());
  EXPECT_FALSE(chr->has_sprites());
  EXPECT_FALSE(contains_id(chr->sprites(), sprite->id()));
}

TEST_F(EntityTests, BasicScriptTest) {
  auto chr =
      std::make_shared<Character>(mock_gen->generate_next(), 100, 100, 100);
  auto scr = std::make_shared<Script>(mock_gen->generate_next(), chr, 0, 0);
  chr->add_script(scr->id());

  BlockDefPtr mock_def = std::make_shared<BlockDefinition>(BlockDefinition(
      mock_gen->generate_next(), "mock_data_id", "move %0 steps", "motion", {},
      {.type = ValueType::NUMBER}));

  auto instance =
      std::make_shared<BlockInstance>(mock_gen->generate_next(), mock_def);

  scr->add_block_instance(instance->id());
  EXPECT_TRUE(scr->has_block_instances());
  EXPECT_TRUE(contains_id(scr->blocks(), instance->id()));

  scr->remove_block_instance(instance->id());

  EXPECT_FALSE(scr->has_block_instances());
  EXPECT_FALSE(contains_id(scr->blocks(), instance->id()));
}

TEST(ProjectManagerTests, ProjectCreationTest) {
  EXPECT_FALSE(mgr.has_project());
  mgr.create();
  EXPECT_TRUE(mgr.has_project());
  EXPECT_TRUE(mgr.project());
  mgr.clear();
}

TEST(ProjectManagerTests, CharacterCreationTests) {
  mgr.create();
  auto dest = std::filesystem::path("./asset_dest/");
  mgr.add_character("./builtin/cat.png", dest);

  // Check for the existance of the new folder
  EXPECT_TRUE(std::filesystem::exists(dest));
}

TEST(ValueTests, BasicTest) {
  Value val(ValueType::NUMBER);
  val.set(3);
  EXPECT_TRUE(int(val) == 3);
  val.set(4);
  EXPECT_TRUE(int(val) == 4);

  Value msg(ValueType::TEXT);
  msg.set("3");
  EXPECT_TRUE(std::string(msg) == "3");
  msg.set("HELLO");
  EXPECT_TRUE(std::string(msg) == "HELLO");
}
