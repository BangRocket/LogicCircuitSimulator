#include "ResourceManager.h"
#include <iostream>

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

void ResourceManager::loadTexture(const std::string& name, const std::string& filename) {
    Texture2D texture = LoadTexture(filename.c_str());
    if (texture.id == 0) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return;
    }
    textures[name] = texture;
}

Texture2D ResourceManager::getTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second;
    }
    std::cerr << "Texture not found: " << name << std::endl;
    return { 0 };
}

void ResourceManager::unloadTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        UnloadTexture(it->second);
        textures.erase(it);
    }
}

void ResourceManager::unloadAllTextures() {
    for (auto& pair : textures) {
        UnloadTexture(pair.second);
    }
    textures.clear();
}

ResourceManager::~ResourceManager() {
    unloadAllTextures();
}