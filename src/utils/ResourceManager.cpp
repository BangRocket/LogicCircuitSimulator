#include "ResourceManager.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include "../../include/nanosvg.h"
#include "../../include/nanosvgrast.h"

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

void ResourceManager::loadSVGTexture(const std::string& name, const std::string& filename, int width, int height) {
    std::cout << "Starting to load SVG texture: " << name << " from file: " << filename << std::endl;

    // Check if file exists
    std::ifstream file(filename);
    if (!file.good()) {
        std::cerr << "SVG file does not exist or is not accessible: " << filename << std::endl;
        return;
    }
    file.close();

    NSVGimage* image = nsvgParseFromFile(filename.c_str(), "px", 96);
    if (image == nullptr) {
        std::cerr << "Failed to parse SVG file: " << filename << std::endl;
        return;
    }

    std::cout << "Successfully parsed SVG file: " << filename << std::endl;
    std::cout << "SVG dimensions: " << image->width << "x" << image->height << std::endl;

    NSVGrasterizer* rast = nsvgCreateRasterizer();
    if (rast == nullptr) {
        std::cerr << "Failed to create SVG rasterizer" << std::endl;
        nsvgDelete(image);
        return;
    }

    // Use std::vector for automatic memory management
    std::vector<unsigned char> pixels(width * height * 4, 0);

    nsvgRasterize(rast, image, 0, 0, 1, pixels.data(), width, height, width * 4);

    std::cout << "Rasterized SVG to " << width << "x" << height << " pixels" << std::endl;

    // Create Image struct without allocating new memory
    Image img = {
        .data = pixels.data(),
        .width = width,
        .height = height,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
        .mipmaps = 1
    };

    std::cout << "Image created with dimensions: " << img.width << "x" << img.height << std::endl;

    Texture2D texture = LoadTextureFromImage(img);
    std::cout << "Texture loaded with ID: " << texture.id << std::endl;

    if (texture.id == 0) {
        std::cerr << "Failed to create texture from SVG: " << filename << std::endl;
    } else {
        textures[name] = texture;
        std::cout << "Successfully created texture for: " << name << std::endl;
    }

    nsvgDeleteRasterizer(rast);
    nsvgDelete(image);

    std::cout << "Finished loading SVG texture: " << name << std::endl;
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