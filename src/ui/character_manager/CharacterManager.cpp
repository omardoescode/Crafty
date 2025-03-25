#include "CharacterManager.h"
#include <imgui.h>
#include <cassert>
#include "utils/images.h"

namespace ui {
CharacterManager::CharacterManager(UIOptions& options) : _options(options) {}
void CharacterManager::draw() {
  // int my_image_width = 0;
  // int my_image_height = 0;
  // GLuint my_image_texture = 0;
  //
  // bool ret = LoadTextureFromFile("assets/icons/image.jpg", &my_image_texture,
  //                                &my_image_width, &my_image_height);
  // assert(ret && "Failed to load the image");
  //
  // ImGui::BeginChild("OpenGL Texture Text");
  // ImGui::Text("pointer = %x", my_image_texture);
  // ImGui::Text("size = %d x %d", my_image_width, my_image_height);
  // ImGui::Image((ImTextureID)(intptr_t)my_image_texture,
  //              ImVec2(my_image_width, my_image_height));
  // ImGui::EndChild();
}
}  // namespace ui
