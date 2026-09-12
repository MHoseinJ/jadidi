#include "Json.h"
#include "core/Log.h"

namespace {
    std::string getTypeName(const nlohmann::json& j) {
        if (j.is_null()) return "null";
        if (j.is_boolean()) return "boolean";
        if (j.is_number_integer()) return "integer";
        if (j.is_number_float()) return "float";
        if (j.is_string()) return "string";
        if (j.is_array()) return "array";
        if (j.is_object()) return "object";
        return "unknown";
    }
}

void Json::throwTypeError(const std::string& field, const std::string& expectedType, const std::string& actualType) const {
    std::string msg = "Type mismatch in field '" + field + "'. Expected " + expectedType + ", but got " + actualType;
    throw JsonException(context_, msg);
}

void Json::throwMissingError(const std::string& field) const {
    std::string msg = "Required field '" + field + "' is missing";
    throw JsonException(context_, msg);
}

template<>
int Json::get<int>(const std::string& field, const int& defaultValue) const {
    if (!json_ || !json_->contains(field)) return defaultValue;
    
    const auto& val = (*json_)[field];
    if (val.is_number_integer()) return val.get<int>();
    if (val.is_number_float()) return static_cast<int>(val.get<float>());
    
    throwTypeError(field, "integer", getTypeName(val));
    return defaultValue;
}

template<>
float Json::get<float>(const std::string& field, const float& defaultValue) const {
    if (!json_ || !json_->contains(field)) return defaultValue;
    
    const auto& val = (*json_)[field];
    if (val.is_number()) return val.get<float>();
    
    throwTypeError(field, "number", getTypeName(val));
    return defaultValue;
}

template<>
std::string Json::get<std::string>(const std::string& field, const std::string& defaultValue) const {
    if (!json_ || !json_->contains(field)) return defaultValue;
    
    const auto& val = (*json_)[field];
    if (val.is_string()) return val.get<std::string>();
    
    throwTypeError(field, "string", getTypeName(val));
    return defaultValue;
}

template<>
bool Json::get<bool>(const std::string& field, const bool& defaultValue) const {
    if (!json_ || !json_->contains(field)) return defaultValue;
    
    const auto& val = (*json_)[field];
    if (val.is_boolean()) return val.get<bool>();
    
    throwTypeError(field, "boolean", getTypeName(val));
    return defaultValue;
}

template<>
Vector2 Json::get<Vector2>(const std::string& field, const Vector2& defaultValue) const {
    if (!json_ || !json_->contains(field)) return defaultValue;
    
    const auto& val = (*json_)[field];
    if (!val.is_object()) {
        throwTypeError(field, "object with x,y", getTypeName(val));
        return defaultValue;
    }
    
    Json vecJson(&val, context_.empty() ? field : context_ + " -> " + field);
    float x = vecJson.get<float>("x", defaultValue.x);
    float y = vecJson.get<float>("y", defaultValue.y);
    
    return Vector2(x, y);
}

template<typename T>
T Json::get(const std::string& field) const {
    if (!json_ || !json_->contains(field)) {
        throwMissingError(field);
    }
    return get<T>(field, T{});
}

bool Json::has(const std::string& field) const {
    return json_ && json_->contains(field);
}

Json Json::getObject(const std::string& field) const {
    if (!json_ || !json_->contains(field)) {
        return Json(nullptr, context_);
    }
    
    const auto& val = (*json_)[field];
    if (!val.is_object()) {
        throwTypeError(field, "object", getTypeName(val));
    }
    
    return Json(&val, context_.empty() ? field : context_ + " -> " + field);
}

size_t Json::size() const {
    if (!json_ || !json_->is_array()) return 0;
    return json_->size();
}

Json Json::operator[](size_t index) const {
    if (!json_ || !json_->is_array() || index >= json_->size()) {
        return Json(nullptr, context_ + "[" + std::to_string(index) + "]");
    }
    
    return Json(&(*json_)[index], context_ + "[" + std::to_string(index) + "]");
}

const nlohmann::json& Json::raw() const {
    if (!json_) {
        throw JsonException(context_, "Attempted to access invalid JSON object");
    }
    return *json_;
}