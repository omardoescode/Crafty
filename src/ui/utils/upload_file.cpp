#include "upload_file.h"
#include <atomic>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "nfd.h"

std::atomic<bool> is_dialog_open = false;
void initialize() {
  NFD_Init();
  atexit([]() { NFD_Quit(); });
}

std::filesystem::path upload_file(const char* filter_list_title,
                                  const char* filter_list,
                                  const char* default_path) {
  static bool initialized = false;
  if (!initialized) {
    initialize();
    initialized = true;
  }
  const nfdu8filteritem_t filter_input[] = {{filter_list_title, filter_list}};

  if (is_dialog_open.exchange(true)) return {};
  nfdchar_t* outPath = NULL;
  nfdresult_t result = NFD_OpenDialog(&outPath, filter_input, 0, default_path);

  is_dialog_open = false;  // Reset flag
  std::cout << outPath << std::endl;

  std::filesystem::path res;

  if (result == NFD_OKAY) {
    res = outPath;
    NFD_FreePathN(outPath);
  } else if (result == NFD_ERROR) {
    assert(false && "Error openning the file");
  }

  return res;
}
