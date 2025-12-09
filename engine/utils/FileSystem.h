#pragma once
#include <string>
#include <vector>

namespace fs {

    bool fileExists(const std::string& path);
    bool createFile(const std::string& path);

    std::vector<std::string> readLines(const std::string& path);
    bool writeLines(const std::string& path, const std::vector<std::string>& lines);

}