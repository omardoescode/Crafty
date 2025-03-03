
#include "MainMenuBar.h"
#include <imgui.h>
namespace ui {
MainMenuBar::MainMenuBar(UIOptions& options) : _options(options) {}
void MainMenuBar::draw() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New Project", "Ctrl+N")) {
      }
      if (ImGui::MenuItem("Open Project...", "Ctrl+O")) {
      }
      if (ImGui::MenuItem("Save Project", "Ctrl+S")) {
      }
      if (ImGui::MenuItem("Save Project As...", "Ctrl+Shift+S")) {
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Exit", "Alt+F4")) _options.close();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
      ImGui::EndMenu();
    }
    // TODO: Change the default style color to your theme
    if (ImGui::BeginMenu("View")) {
      if (ImGui::BeginMenu("Theme")) {
        if (ImGui::MenuItem("Light Theme")) ImGui::StyleColorsLight();
        if (ImGui::MenuItem("Dark Theme")) ImGui::StyleColorsDark();
        if (ImGui::MenuItem("Classic Theme")) ImGui::StyleColorsClassic();
        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Project")) {
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help")) {
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}
}  // namespace ui
