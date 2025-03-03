
#include "MainMenuBar.h"
namespace ui {
MainMenuBar::MainMenuBar(UIOptions& options) : _options(options) {}
void MainMenuBar::draw() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New Project")) {
      }
      if (ImGui::MenuItem("Open Project...")) {
      }
      if (ImGui::MenuItem("Save Project")) {
      }
      if (ImGui::MenuItem("Save Project As...")) {
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Exit", "Alt+F4")) {
        _options.close();
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
    }
    if (ImGui::BeginMenu("View")) {
    }
    if (ImGui::BeginMenu("Project")) {
    }
    if (ImGui::BeginMenu("Help")) {
    }
    ImGui::EndMainMenuBar();
  }

  ImGui::End();  // End main application window
}
}  // namespace ui
