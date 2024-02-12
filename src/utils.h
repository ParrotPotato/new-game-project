#ifndef UTILS
#define UTILS

#include <string>
#include <optional>

namespace utils{
    std::optional<std::string> load_file_buffer(const char * file_path);
}

#endif
