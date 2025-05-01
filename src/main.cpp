#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <format>
#include <memory>
#include "action_deferrer.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl3.h"
#include "character/character_manager.h"
#include "editor/block_canvas.h"
#include "editor/block_picker.h"
#include "imgui.h"
#include "logic/lua_impl/lua_interpreter.h"
#include "project_manager.h"
#include "stage/stage.h"
#include "ui/editor/block_category_panel.h"
#include "ui/main_menu_bar.h"
#include "ui_options.h"
#include "utils/material_symbols.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

int main(int args, char **argv) {
  std::format_string<std::string &> lua_path = "lua/{}.lua";
  std::unique_ptr<logic::Interpreter> interpreter =
      std::make_unique<logic::lua::LuaInterpreter>(lua_path);
  ui::UIOptions options(args, argv, std::move(interpreter));

  // Initialize SDL3
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    printf(" %s\n", SDL_GetError());
    return -1;
  }
  // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100 (WebGL 1.0)
  const char *glsl_version = "#version 100";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
  // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
  const char *glsl_version = "#version 300 es";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
  // GL 3.2 Core + GLSL 150
  const char *glsl_version = "#version 150";
  SDL_GL_SetAttribute(
      SDL_GL_CONTEXT_FLAGS,
      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);  // Always required on Mac
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_Renderer *renderer;
  SDL_Window *window;
  if (!SDL_CreateWindowAndRenderer("Crafty", 1500, 1000,
                                   SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE,
                                   &window, &renderer)) {
    printf(" SDL_CreateWindowAndRenderer(): %s\n", SDL_GetError());
    SDL_Quit();
    return -1;
  }
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  if (gl_context == nullptr) {
    printf(" SDL_GL_CreateContext(): %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1);  // Enable vsync
  SDL_ShowWindow(window);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard |
                    ImGuiConfigFlags_NavEnableGamepad |
                    ImGuiConfigFlags_DockingEnable;

  // Initialize ImGui backends for SDL3 + OpenGL3
  ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init("#version 130");

  // Load fonts (make sure the file exists in the specified path)
  options.initialize_font(options.DEFAULT_FONT, "assets/fonts/Rubik.ttf",
                          18.0f);
  ImFontConfig config;
  config.PixelSnapH = true;
  config.GlyphOffset.y += 1;
  static const ImWchar icon_ranges[] = {ICON_MIN_MD, ICON_MAX_MD, 0};
  options.initialize_font(options.ICONS_FONT_MEDIUM,
                          "assets/fonts/MaterialIcons-Regular.ttf", 20.0f,
                          &config, icon_ranges);
  ImVec4 clear_color = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);

  // Initialize the project
  auto &prj_mgr = model::ProjectManager::instance();
  prj_mgr.create();
  auto prj = prj_mgr.project();

  // Initialize UI components
  ui::MainMenuBar main_menu_bar(options);
  ui::BlockCategoryPanel block_category_panel(options);
  ui::BlockPicker picker(options);
  ui::BlockCanvas canvas(options);
  ui::Stage stage(options, renderer);
  ui::CharacterManager character_mgr(options);

  // ImGui Style
  ImGuiStyle &style = ImGui::GetStyle();

  // Main loop
  while (options.running()) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL3_ProcessEvent(&event);

      // Process quit events: SDL_EVENT_QUIT or window close events
      if (event.type == SDL_EVENT_QUIT) {
        options.close();
      }
      // TODO: Handle Resize later
    }

    // If the window is minimized, delay and continue
    if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
      SDL_Delay(10);
      continue;
    }

    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // Show demo window for testing purposes
    ImGui::ShowDemoWindow();

    // Create a full-viewport window
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::Begin("CraftyMainWindow", NULL, window_flags);

    // Draw UI components
    main_menu_bar.draw();

    // Left Sidebar
    ImGui::BeginChild(
        "Left Sidebar", ImVec2(options.LEFT_SIDEBAR_WIDTH, -1),
        ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY);
    block_category_panel.draw();
    ImGui::Separator();
    picker.draw();
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("Canvas", ImVec2(ImGui::GetContentRegionAvail().x -
                                           options.RIGHT_SIDEBAR_WIDTH,
                                       0));
    canvas.draw();
    ImGui::EndChild();

    ImGui::SameLine();

    int right_width = options.RIGHT_SIDEBAR_WIDTH - style.WindowPadding.x;
    ImGui::BeginChild("RightSidebar", ImVec2(right_width, 0));
    ImGui::BeginChild("Stage",
                      ImVec2(right_width, right_width / options.STAGE_ASPECT),
                      ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar);
    stage.draw();
    ImGui::EndChild();
    character_mgr.draw();

    ImGui::EndChild();

    if (ImGui::Button("Hello")) ImGui::OpenPopup("test");

    if (ImGui::BeginPopupModal("test", NULL,
                               ImGuiWindowFlags_AlwaysAutoResize)) {
      ImGui::SeparatorText("Aquarium");
      ImGui::Text("HELLO??");
      ImGui::EndPopup();
    }

    ImGui::End();

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);

    // Flushing Actions
    auto &deferrer = ui::ActionDeferrer::instance();
    deferrer.flush();
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
  SDL_GL_DestroyContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
