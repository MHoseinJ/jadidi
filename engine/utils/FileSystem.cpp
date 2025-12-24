#include "FileSystem.h"
#include "core/Log.h"

#include <fstream>
#include <sstream>

using json = nlohmann::json;

namespace fs {

    bool fileExists(const std::string& path) {
        std::ifstream f(path);
        return f.good();
    }

    bool createFile(const std::string& path) {
        std::ofstream f(path);
        return f.good();
    }

    std::vector<std::string> readLines(const std::string& path) {
        std::vector<std::string> lines;
        std::ifstream file(path);

        if (!file) {
            gameLog(("File not found: " + path).c_str(), ERROR);
            return lines;
        }

        std::string line;
        while (std::getline(file, line))
            lines.push_back(line);

        return lines;
    }

    bool writeLines(const std::string& path, const std::vector<std::string>& lines) {
        std::ofstream file(path);

        if (!file) {
            gameLog(("Cannot write to file: " + path).c_str(), ERROR);
            return false;
        }

        for (auto& line : lines)
            file << line << "\n";

        return true;
    }

    json readJson(const std::string& path) {
        std::ifstream file(path);

        if (!file) {
            gameLog(("JSON file missing: " + path).c_str(), ERROR);
            return json();
        }

        json data;
        try {
            file >> data;
        }
        catch (std::exception& e) {
            gameLog(("JSON parse error in " + path + ": " + e.what()).c_str(), ERROR);
            return json();
        }

        return data;
    }

    bool writeJson(const std::string& path, const json& data) {
        std::ofstream file(path);

        if (!file) {
            gameLog(("Cannot save JSON file: " + path).c_str(), ERROR);
            return false;
        }

        file << data.dump(4);
        return true;
    }

    std::vector<std::string> listFiles(const std::string& directory) {
        std::vector<std::string> files;

        try {
            for (const auto& entry : std::filesystem::directory_iterator(directory)) {
                if (entry.is_regular_file()) {
                    files.push_back(entry.path().string());
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            gameLog(("Cannot list files: " + directory).c_str(), ERROR);
        }

        return files;
    }

}