#pragma once
#include "json.hpp"
#include <string>
#include <stdexcept>
#include <memory>
#include "utils/math/vector.h"

class JsonException : public std::runtime_error {
public:
    JsonException(const std::string& context, const std::string& message)
        : std::runtime_error("[JSON Error] " + context + ": " + message),
          context_(context), message_(message) {}
    
    const std::string& getContext() const { return context_; }
    const std::string& getMessage() const { return message_; }

private:
    std::string context_;
    std::string message_;
};

class Json {
public:
    Json() : json_(nullptr), jsonOwned_(nullptr), context_("") {}
    
    Json(const nlohmann::json* json, const std::string& context = "")
        : json_(json), jsonOwned_(nullptr), context_(context) {}
    
    Json(std::shared_ptr<const nlohmann::json> json, const std::string& context = "")
        : json_(json.get()), jsonOwned_(std::move(json)), context_(context) {}
    
    Json(std::shared_ptr<const nlohmann::json> owner, 
         const nlohmann::json* ptr, 
         const std::string& context)
        : json_(ptr), jsonOwned_(std::move(owner)), context_(context) {}
    
    Json withContext(const std::string& additionalContext) const {
        std::string newContext = context_.empty() ? additionalContext : context_ + " -> " + additionalContext;
        if (jsonOwned_) {
            return Json(jsonOwned_, json_, newContext);
        }
        return Json(json_, newContext);
    }
    
    template<typename T>
    T get(const std::string& field, const T& defaultValue = T{}) const;
    
    template<typename T>
    T getRequired(const std::string& field) const;
    
    bool has(const std::string& field) const;
    Json getObject(const std::string& field) const;
    Json getArray(const std::string& field) const;
    size_t size() const;
    Json operator[](size_t index) const;
    const nlohmann::json& raw() const;
    bool isValid() const { return json_ != nullptr && !json_->is_null(); }

private:
    const nlohmann::json* json_;
    std::shared_ptr<const nlohmann::json> jsonOwned_;
    std::string context_;
    
    void throwTypeError(const std::string& field, const std::string& expectedType, const std::string& actualType) const;
    void throwMissingError(const std::string& field) const;
    
    Json createChild(const nlohmann::json* childPtr, const std::string& childContext) const {
        if (jsonOwned_) {
            return Json(jsonOwned_, childPtr, childContext);
        }
        return Json(childPtr, childContext);
    }
};

// template specializations
template<> int Json::get<int>(const std::string& field, const int& defaultValue) const;
template<> float Json::get<float>(const std::string& field, const float& defaultValue) const;
template<> std::string Json::get<std::string>(const std::string& field, const std::string& defaultValue) const;
template<> bool Json::get<bool>(const std::string& field, const bool& defaultValue) const;
template<> Vector2 Json::get<Vector2>(const std::string& field, const Vector2& defaultValue) const;

template<> int Json::getRequired<int>(const std::string& field) const;
template<> float Json::getRequired<float>(const std::string& field) const;
template<> std::string Json::getRequired<std::string>(const std::string& field) const;
template<> bool Json::getRequired<bool>(const std::string& field) const;
template<> Vector2 Json::getRequired<Vector2>(const std::string& field) const;