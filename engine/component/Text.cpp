#include "Text.h"
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
    if (texture.sdlTexture) {
        ITextureBackend* backend = TextureManager::instance().getBackend();
        if (backend) {
            backend->destroyTexture(texture);
        }
    }
}

void Text::DeSerialize(const json &j) {
    if (j.find("text") != j.end()) {
        text = j["text"];
    }
    
    if (j.find("color") != j.end()) {
        color.r = j["color"]["r"];
        color.g = j["color"]["g"];
        color.b = j["color"]["b"];
        color.a = j["color"]["a"];
    } else {
        color = {0, 0, 0, 255};
    }

    fontSize = j.value("size", 16);
    fontName = j.value("font", "font");
}

void Text::Reload() {
    OnDestroy();
    OnCreate();
}