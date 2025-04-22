#include <cstddef>
#include "SDL3/SDL_opengl.h"
#include "imgui.h"
bool LoadTextureFromMemory(const void* data, size_t data_size,
                           GLuint* out_texture, int* out_width,
                           int* out_height);

bool LoadTextureFromFile(const char* file_name, GLuint* out_texture,
                         int* out_width, int* out_height);

void ImageRotated(ImTextureID tex_id, ImVec2 center, ImVec2 size, float angle);
