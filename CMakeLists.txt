cmake_minimum_required(VERSION 3.18)
project(crafty)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# Set directory paths
set(UI_DIR ${CMAKE_SOURCE_DIR}/src/ui) 
set(MODEL_DIR ${CMAKE_SOURCE_DIR}/src/model)
set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/src/thirdparty) 

# Get opengl from system
# TODO: Change to use FetchContent Instead too
find_package(OpenGL REQUIRED)

# Import FetchContent
include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)

# Fetch SDL
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
  GIT_TAG release-3.2.8
  GIT_PROGRESS TRUE
)
message(STATUS "Fetching SDL ...")
FetchContent_MakeAvailable(SDL3)

# Fetch IMGUI, docking version
FetchContent_Declare(
  imgui
  GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
  GIT_TAG docking
  GIT_PROGRESS TRUE
)

message(STATUS "Fetching imgui ...")
FetchContent_MakeAvailable(imgui)
# Fetching json reader
FetchContent_Declare(
  json 
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
  GIT_TAG master
  GIT_PROGRESS TRUE
)

message(STATUS "Fetching nlohmann/json ...")
FetchContent_MakeAvailable(json)

# Fetching json reader
FetchContent_Declare(
  nfd
  GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended
  GIT_TAG        master  
)

message(STATUS "Fetching nativefiledialong-extended ...")
set(NFD_BUILD_TESTS OFF CACHE BOOL "Disable tests")
set(NFD_PORTAL OFF CACHE BOOL "Disable Flatpak portal (Linux)")
FetchContent_MakeAvailable(nfd)

# Fetch GTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
message(STATUS "Fetching GTest ...")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Build a library for model
add_library(model
  ${MODEL_DIR}/block/block_library.cpp
  ${MODEL_DIR}/block/block_instance.cpp
  ${MODEL_DIR}/block/block_definition.cpp

  ${MODEL_DIR}/utils/fs.cpp
  ${MODEL_DIR}/utils/json.cpp
  ${MODEL_DIR}/utils/serializable.cpp
  ${MODEL_DIR}/utils/ID_manager.cpp

  ${MODEL_DIR}/project.cpp
  ${MODEL_DIR}/project_manager.cpp
  ${MODEL_DIR}/asset.cpp
  ${MODEL_DIR}/script.cpp
  ${MODEL_DIR}/character.cpp
)

target_link_libraries(model PUBLIC nlohmann_json::nlohmann_json)
target_include_directories(model PUBLIC ${MODEL_DIR}) # encapsulate model dir but only the public interface

# Building the ImGUI library
add_library(imgui_sdl
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui.h
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

# Build a library for ui
add_library(ui
  ${UI_DIR}/ui_options.cpp 
  ${UI_DIR}/main_menu_bar.cpp 

  ${UI_DIR}/editor/block_category_panel.cpp 
  ${UI_DIR}/editor/block_picker.cpp 
  ${UI_DIR}/editor/block_canvas.cpp 
  ${UI_DIR}/editor/block_view.cpp 

  ${UI_DIR}/stage/stage.cpp 

  ${UI_DIR}/character_manager/character_manager.cpp 

  ${UI_DIR}/utils/images.cpp
  ${UI_DIR}/utils/file_utils.cpp
  ${UI_DIR}/utils/platform.cpp
)
target_link_libraries(ui PUBLIC imgui_sdl model nfd)
target_include_directories(ui PUBLIC ${UI_DIR} ${THIRDPARTY_DIR})
target_include_directories(ui PUBLIC ${UI_DIR})
# The executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(crafty src/main.cpp)
target_link_libraries(crafty PUBLIC ui)

# Testing
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}) # Back to normal
enable_testing()

add_executable(tests tests/ModelLayerTests.cpp)
target_link_libraries(tests GTest::gtest_main model)
target_include_directories(tests PRIVATE 
  ${MODEL_DIR}
)
include(GoogleTest)
gtest_discover_tests(tests)

# Copy assets
message("Copying assets and data")
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(COPY data DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(COPY builtin DESTINATION ${CMAKE_BINARY_DIR}/bin)
message("Finishing copying")
