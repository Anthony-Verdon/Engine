#include "Engine/3D/ModelLoader/ModelLoader.hpp"
#include "GlbParser/GlbParser.hpp"
#include "Json/Json.hpp"
#include "Engine/RessourceManager/RessourceManager.hpp"
#include "Toolbox.hpp"
#include <iostream>

namespace ModelLoader
{
    std::vector<Model> LoadModel(const std::string &path)
    {
        if (Toolbox::checkExtension(path, ".glb"))
            return (LoadModelFromGlb(path));
        else if (Toolbox::checkExtension(path, ".gltf"))
        return (LoadModelFromGltf(path));
        
        throw(std::runtime_error("unknown extension"));
    }

    std::vector<Model> LoadModelFromGlb(const std::string &path)
    {
        auto [gltfJson, binStr] = Glb::LoadBinaryFile(path);

        Glb::GltfData data = Glb::LoadGltf(gltfJson, binStr);
        Glb::Scene scene = data.scenes[data.rootScene];

        std::vector<Model> models;
        for (size_t i = 0; i < scene.nodes.size(); i++)
            models.push_back({data, (size_t)scene.nodes[i]});

        return (models);
    }

    std::vector<Model> LoadModelFromGltf(const std::string &path)
    {
        std::string gltfStr = Toolbox::readFile(path);
        auto it = gltfStr.begin();
        auto gltfJson = Json::ParseJson(gltfStr, it);
        
        std::string pathToGltf = path.substr(0, path.find('/') + 1);
        auto binStr = Toolbox::readFile(pathToGltf + std::string(gltfJson["buffers"][0]["bin"]), std::ios::binary);

        Glb::GltfData data = Glb::LoadGltf(gltfJson, binStr);
        Glb::Scene scene = data.scenes[data.rootScene];

        std::vector<Model> models;
        for (size_t i = 0; i < scene.nodes.size(); i++)
            models.push_back({data, (size_t)scene.nodes[i]});

        return (models);
    }
}