cmake_minimum_required(VERSION 3.16)
project(crafty)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# Set directory paths
set(UI_DIR ${CMAKE_SOURCE_DIR}/src/ui)
set(MODEL_DIR ${CMAKE_SOURCE_DIR}/src/model)

# Get opengl from system
# TODO: Chagne to use FetchContent Instead too
find_package(OpenGL REQUIRED)
include(FetchContent)

# Fetch SDL
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
  GIT_TAG release-2.0.22
)
message(STATUS "Fetching SDL ...")
FetchContent_MakeAvailable(SDL2)

# Fetch IMGUI, docking version
FetchContent_Declare(
  imgui
  GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
  GIT_TAG docking
)

message(STATUS "Fetching imgui ...")
FetchContent_MakeAvailable(imgui)
# Fetching json reader
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)

message(STATUS "Fetching nlohmann/json ...")
FetchContent_MakeAvailable(json)

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
  ${MODEL_DIR}/block/BlockLibrary.cpp
  ${MODEL_DIR}/utils/fs.cpp
)

target_link_libraries(model PUBLIC nlohmann_json::nlohmann_json)
target_include_directories(model PUBLIC ${MODEL_DIR})

# Building the ImGUI library
add_library(imgui
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
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.h
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.h
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
target_link_libraries(imgui PUBLIC SDL2::SDL2 OpenGL::GL)

# Build a library for ui
add_library(ui
  ${UI_DIR}/ui_options.cpp 
  ${UI_DIR}/MainMenuBar.cpp 
  ${UI_DIR}/BlockCategoryPanel.cpp 
)
target_link_libraries(ui PUBLIC imgui model) # model for reading some data
target_include_directories(ui PUBLIC ${UI_DIR} ${MODEL_DIR})
# The executable
add_executable(crafty src/main.cpp)

target_link_libraries(crafty PUBLIC ui)


# Testing
enable_testing()

add_executable(tests tests/ModelLayerTests.cpp)
target_link_libraries(tests GTest::gtest_main model)
target_include_directories(tests PUBLIC 
  ${MODEL_DIR}
)
include(GoogleTest)
gtest_discover_tests(tests)

# Copy assets
message("Copying assets and data")
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY data DESTINATION ${CMAKE_BINARY_DIR})
message("Finishing copying")
