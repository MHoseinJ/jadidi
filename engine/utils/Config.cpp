#include "Config.h"
#include "core/Log.h"
#include <fstream>
#include <iostream>

Config::Config(const std::string& path) : m_path(path) {}

bool Config::load() {
    std::ifstream file(m_path);

    // checks if file not exists the quit
    if (!file) {
        gameLog((std::string("[Config] Config file not found: ") + m_path).c_str(), WARNING);
        return false;
    }

    try {
        // extract the json from ifstream
        file >> m_json;
    } catch (std::exception& e) {
        gameLog(std::string("[Config] JSON parse error: ") + e.what(), ERROR);
        return false;
    }

    gameLog((std::string("[Config] Loaded config: ") + m_path).c_str(), INFO);
    return true;
}

bool Config::save() const {
    std::ofstream file(m_path);

    // if file can't be opened so we return false
    if (!file) {
        gameLog((std::string("[Config] Failed to save file: ") + m_path).c_str(), ERROR);
        return false;
    }

    // extract and beautify json
    file << m_json.dump(4);

    gameLog((std::string("[Config] Saved config: ") + m_path).c_str(), INFO);
    return true;
}

json& Config::data() {
    return m_json;
}

const json& Config::data() const {
    return m_json;
}