#include "secure_buffer.h"
#include <nlohmann/json.hpp>
#include <filesystem>

namespace fs = std::filesystem;

auto convert_from_bitwarden_json(const fs::path& json_path) -> SecureBuffer;
