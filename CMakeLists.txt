cmake_minimum_required(VERSION 3.18)
project(crafty)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_BUILD_TYPE DEBUG)
set(GIT_VERBOSE false)

# Set directory paths
set(UI_DIR ${CMAKE_SOURCE_DIR}/src/ui) 
set(MODEL_DIR ${CMAKE_SOURCE_DIR}/src/model)
set(LOGIC_DIR ${CMAKE_SOURCE_DIR}/src/logic)
set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/src/thirdparty) 
set(COMMON_DIR ${CMAKE_SOURCE_DIR}/src/common) 

# Get opengl from system
find_package(OpenGL REQUIRED)

find_package(Lua REQUIRED)

# Import FetchContent
include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)

# Fetch SDL
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
  GIT_TAG release-3.2.8
  GIT_PROGRESS ${GIT_VERBOSE}
)
message(STATUS "Fetching SDL ...")
FetchContent_MakeAvailable(SDL3)

# Fetch IMGUI, docking version
FetchContent_Declare(
  imgui
  GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
  GIT_TAG docking
  GIT_PROGRESS ${GIT_VERBOSE}
)

message(STATUS "Fetching imgui ...")
FetchContent_MakeAvailable(imgui)
# Fetching json reader
FetchContent_Declare(
  json 
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
  GIT_TAG master
  GIT_PROGRESS ${GIT_VERBOSE}
)

message(STATUS "Fetching nlohmann/json ...")
FetchContent_MakeAvailable(json)

# Fetching json reader
FetchContent_Declare(
  nfd
  GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended
  GIT_TAG        master  
  GIT_PROGRESS ${GIT_VERBOSE}
)

message(STATUS "Fetching nativefiledialong-extended ...")
set(NFD_BUILD_TESTS OFF CACHE BOOL "Disable tests")
set(NFD_PORTAL OFF CACHE BOOL "Disable Flatpak portal (Linux)")
FetchContent_MakeAvailable(nfd)

# Fetching SOL2
FetchContent_Declare(
  sol2
  GIT_REPOSITORY https://github.com/ThePhD/sol2
  GIT_TAG        main
  GIT_PROGRESS ${GIT_VERBOSE}
)
message(STATUS "Fetching sol2 ...")
FetchContent_MakeAvailable(sol2)

# Fetch GTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  GIT_PROGRESS ${GIT_VERBOSE}
)
message(STATUS "Fetching GTest ...")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Build a library for common
add_library(common
  ${COMMON_DIR}/events/event_dispatcher.cpp
  ${COMMON_DIR}/logger/logger.cpp
  ${COMMON_DIR}/common_logger.cpp
)

target_include_directories(common PUBLIC ${COMMON_DIR}) # encapsulate model dir but only the public interface

# Build a library for model
add_library(model
  ${MODEL_DIR}/block/block_library.cpp
  ${MODEL_DIR}/block/block_instance.cpp
  ${MODEL_DIR}/block/block_definition.cpp
  ${MODEL_DIR}/block/json_block_storage.cpp
  ${MODEL_DIR}/block/input_slot.cpp
  ${MODEL_DIR}/block/value.cpp
  ${MODEL_DIR}/block/value_type.cpp
  ${MODEL_DIR}/block/input_slot_instance.cpp


  # identity/id
  ${MODEL_DIR}/identity/id.cpp
  ${MODEL_DIR}/identity/prefixed_id_generator.cpp
  ${MODEL_DIR}/identity/string_id.cpp
  ${MODEL_DIR}/identity/identifiable.cpp

  # utils
  ${MODEL_DIR}/utils/fs.cpp
  ${MODEL_DIR}/utils/json.cpp

  # Serialization
  ${MODEL_DIR}/serialization/serializable.cpp

  # error
  ${MODEL_DIR}/error/type_mismatch.cpp

  ${MODEL_DIR}/project.cpp
  ${MODEL_DIR}/project_manager.cpp
  ${MODEL_DIR}/asset.cpp
  ${MODEL_DIR}/script.cpp
  ${MODEL_DIR}/character.cpp
  ${MODEL_DIR}/model_logger.cpp
)

target_link_libraries(model PUBLIC common nlohmann_json::nlohmann_json)
target_include_directories(model PUBLIC ${MODEL_DIR} ${COMMON_DIR}) # encapsulate model dir but only the public interface

# Building the ImGUI library
add_library(imgui_sdl
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui.h
  ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
  ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h
  ${imgui_SOURCE_DIR}/imconfig.h
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_internal.h
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imstb_rectpack.h
  ${imgui_SOURCE_DIR}/imstb_textedit.h
  ${imgui_SOURCE_DIR}/imstb_truetype.h
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.h
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.h
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui_sdl PUBLIC ${imgui_SOURCE_DIR})
target_link_libraries(imgui_sdl PUBLIC SDL3::SDL3 OpenGL::GL)

# Build a library for logic
add_library(logic
  ${LOGIC_DIR}/logic_logger.cpp 
  ${LOGIC_DIR}/scope_table.cpp

  # Lua Implementation
  ${LOGIC_DIR}/lua_impl/lua_interpreter.cpp 

  # error
  ${LOGIC_DIR}/error/value_not_found.cpp
)
target_link_libraries(logic PUBLIC common model sol2::sol2 ${LUA_LIBRARIES})
target_include_directories(logic PUBLIC ${LOGIC_DIR} ${COMMON_DIR})

# Build a library for ui
add_library(ui
  ${UI_DIR}/ui_options.cpp 
  ${UI_DIR}/ui_logger.cpp 
  ${UI_DIR}/main_menu_bar.cpp 
  ${UI_DIR}/action_deferrer.cpp 

  ${UI_DIR}/editor/block_category_panel.cpp 
  ${UI_DIR}/editor/block_picker.cpp 
  ${UI_DIR}/editor/block_canvas.cpp 
  ${UI_DIR}/editor/block_view.cpp 
  ${UI_DIR}/editor/script_view.cpp 

  ${UI_DIR}/stage/stage.cpp 

  ${UI_DIR}/character/character_view.cpp 
  ${UI_DIR}/character/character_miniview.cpp 
  ${UI_DIR}/character/character_manager.cpp 

  ${UI_DIR}/utils/images.cpp
  ${UI_DIR}/utils/file_utils.cpp
  ${UI_DIR}/utils/platform.cpp
)
target_link_libraries(ui PUBLIC common imgui_sdl model nfd logic)
target_include_directories(ui PUBLIC ${UI_DIR} ${THIRDPARTY_DIR} ${COMMON_DIR} ${LOGIC_DIR})

add_executable(crafty src/main.cpp)
target_link_libraries(crafty PUBLIC ui)

# Testing
enable_testing()

add_executable(tests 
  tests/ModelLayerTests.cpp 
  tests/CommonLayerTests.cpp
  tests/UILayerTests.cpp
  tests/LogicLayerTests.cpp
)
target_link_libraries(tests GTest::gtest_main 
  model 
  common 
  logic
  ui
)
target_include_directories(tests PRIVATE 
  ${MODEL_DIR}
  ${COMMON_DIR}
  ${LOGIC_DIR}
  ${UI_DIR}
)
include(GoogleTest)
gtest_discover_tests(tests)

# Copy assets
message("Copying assets and data")
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY data DESTINATION ${CMAKE_BINARY_DIR})
file(COPY lua DESTINATION ${CMAKE_BINARY_DIR})
file(COPY builtin DESTINATION ${CMAKE_BINARY_DIR})
message("Finishing copying")
