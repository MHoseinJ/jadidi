#pragma once
#include "json.hpp"
#include <string>

using json = nlohmann::json;

class Config {
  public:
    Config(const std::string& path);

    bool load();
    bool save() const;

    json& data();
    const json& data() const;

  private:
    std::string m_path;
    json m_json;
};