#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "raylib.h"
#include <unordered_map>
#include <string>

// Forward declarations
struct NSVGimage;
struct NSVGrasterizer;

class ResourceManager {
public:
    static ResourceManager& getInstance();

    void loadSVGTexture(const std::string& name, const std::string& filename, int width, int height);
    Texture2D getTexture(const std::string& name);
    void unloadTexture(const std::string& name);
    void unloadAllTextures();

private:
    ResourceManager() = default;
    ~ResourceManager();
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    std::unordered_map<std::string, Texture2D> textures;
};

#endif // RESOURCE_MANAGER_H