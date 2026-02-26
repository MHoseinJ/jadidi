#pragma once
#include <string>
#include <unordered_map>
#include <sol/sol.hpp>

class State {
    public:

    static State& instance();

    void set(const std::string& name, const sol::object& value);
    [[nodiscard]]
    sol::object get(const std::string &name) const;
    [[nodiscard]]
    bool exists(const std::string& name) const;
    void remove(const std::string& name);
    void clear();

private:
    std::unordered_map<std::string, sol::object> data;
};
