cmake_minimum_required(VERSION 3.16)
project(crafty)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# Set directory paths
set(UI_DIR ${CMAKE_SOURCE_DIR}/src/ui)

find_package(OpenGL REQUIRED)
include(FetchContent)
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
  GIT_TAG release-2.0.22
)
message(STATUS "Fetching SDL ...")
FetchContent_MakeAvailable(SDL2)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
  GIT_TAG cb8ead1f7198924f97e52973d115e1d4eaeda2f3
)

message(STATUS "Fetching imgui ...")
FetchContent_MakeAvailable(imgui)


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
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl.h
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer.h
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
target_link_libraries(imgui PUBLIC SDL2::SDL2 OpenGL::GL)

# Build a library for ui
add_library(ui
  ${UI_DIR}/ui_options.cpp 
  ${UI_DIR}/MainMenuBar.cpp 
)

# The executable
add_executable(crafty src/main.cpp)

target_link_libraries(crafty PUBLIC imgui ui)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

# Copy assets
add_custom_command(TARGET crafty PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:crafty>/assets)
