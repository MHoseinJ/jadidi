#pragma once
#include <string>
#include <vector>
#include "json.hpp"

namespace fs {

    bool fileExists(const std::string& path);
    bool createFile(const std::string& path);

    std::vector<std::string> readLines(const std::string& path);
    bool writeLines(const std::string& path, const std::vector<std::string>& lines);

    nlohmann::json readJson(const std::string& path);
    bool writeJson(const std::string& path, const nlohmann::json& data);

    std::vector<std::string> listFiles(const std::string& directory);

}