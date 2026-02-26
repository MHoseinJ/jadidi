#include "State.h"

#include "Log.h"

State &State::instance() {
    static State instance;
    return instance;
}

void State::set(const std::string &name, const sol::object &value) {
    data[name] = value;
}

sol::object State::get(const std::string &name) const {
    const auto it = data.find(name);
    if (it == data.end()) {
        gameLog("Nothing found with that key", ERROR);
        return sol::nil;
    }

    return it->second;
}

bool State::exists(const std::string &name) const {
    return data.find(name) != data.end();
}

void State::remove(const std::string &name) {
    data.erase(name);
}

void State::clear() {
    data.clear();
}