#include <nlohmann/json.hpp>
#include <filesystem>
#include "secure_buffer.h"

namespace fs = std::filesystem;

auto convert_from_bitwarden_json(fs::path json_path) -> SecureBuffer;
