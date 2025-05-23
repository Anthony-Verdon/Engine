#pragma once

#include "GlbParser/GlbParser.hpp"
#include "Engine/3D/Lights/Lights.hpp"

class Mesh
{
    public:
        Mesh(const Glb::GltfData &data, size_t nodeIndex);
        ~Mesh();

        void Init();
        void Destroy();
        void Draw(const ml::vec3 &camPos, const std::vector<std::unique_ptr<ALight>> &lights, const ml::mat4 &projection, const ml::mat4 &view, std::map<int, ml::mat4> &nodesTransform);

    private:
        std::string name;
        std::vector<unsigned int> VAOs;
        std::vector<unsigned int> VBOs;
        std::vector<unsigned int> EBOs;
        std::vector<Glb::Primitive> primitives;
        std::map<int, Glb::Material> materials;
        std::map<int, std::string> baseColorTextures;
        std::vector<Glb::Joint> joints;
        
        size_t nodeIndex;

        static bool shaderInitialized;
};