#include "FileSystem.h"
#include "core/Log.h"
#include <fstream>
#include <iostream>

namespace fs {

    bool fileExists(const std::string& path) {
        std::ifstream f(path);
        return f.good();
    }

    bool createFile(const std::string& path) {
        std::ofstream f(path);

        if (!f) {
            gameLog((std::string("[FileSystem] Failed to create file: ") + path).c_str(), ERROR);
            return false;
        }

        gameLog((std::string("[FileSystem] File created: ") + path).c_str(), INFO);
        return true;
    }

    std::vector<std::string> readLines(const std::string& path) {
        std::vector<std::string> lines;
        std::ifstream file(path);

        if (!file) {
            gameLog((std::string("[FileSystem] Cannot read file: ") + path).c_str(), ERROR);
            return lines;
        }

        std::string line;
        while (std::getline(file, line))
            lines.push_back(line);

        gameLog((std::string("[FileSystem] Loaded lines from: ") + path).c_str(), INFO);
        return lines;
    }

    bool writeLines(const std::string& path, const std::vector<std::string>& lines) {
        std::ofstream file(path);

        if (!file) {
            gameLog((std::string("[FileSystem] Cannot write to file: ") + path).c_str(), ERROR);
            return false;
        }

        for (auto& line : lines)
            file << line << "\n";

        gameLog((std::string("[FileSystem] Written lines to: ") + path).c_str(), INFO);
        return true;
    }
}