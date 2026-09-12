#include "Text.h"
#include "render/ITextureBackend.h"
#include "render/TextureManager.h"

void Text::OnCreate() {
    if (!text.empty() && fontSize && !fontName.empty()) {
        texture = createTextureWithText(text, renderer, color, fontName, fontSize);
    }
    if (texture.isValid()) {
        srcRect.w = texture.width;
        srcRect.h = texture.height;
    }
}

void Text::OnDestroy() {
    if (texture.isValid()) {
        ITextureBackend* backend = TextureManager::instance().getBackend();
        if (backend) {
            backend->destroyTexture(texture);
        }
    }
}

void Text::DeSerialize(const Json& j) {
    text = j.get<std::string>("text", "");
    
    if (j.has("color")) {
        Json colorJson = j.getObject("color");
        color.r = colorJson.get<int>("r", 0);
        color.g = colorJson.get<int>("g", 0);
        color.b = colorJson.get<int>("b", 0);
        color.a = colorJson.get<int>("a", 255);
    } else {
        color = {0, 0, 0, 255};
    }
    
    fontSize = j.get<int>("size", 16);
    fontName = j.get<std::string>("font", "font");
}

void Text::Reload() {
    OnDestroy();
    OnCreate();
}