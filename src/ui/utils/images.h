#include <cstddef>
#include "SDL3/SDL_opengl.h"
bool LoadTextureFromMemory(const void* data, size_t data_size,
                           GLuint* out_texture, int* out_width,
                           int* out_height);

bool LoadTextureFromFile(const char* file_name, GLuint* out_texture,
                         int* out_width, int* out_height);
