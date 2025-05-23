#pragma once

#include "GlbParser/GlbParser.hpp"
#include "Engine/3D/Animator3D/Animator3D.hpp"
#include "Engine/3D/Mesh/Mesh.hpp"

class Model;

struct NodeModel
{
    std::string name;
    ml::mat4 transform;
    std::vector<int> children;
    std::vector<int> models;
};

class Model
{
    public:
        Model();
        Model(const Glb::GltfData &data, size_t nodeIndex);
        ~Model();

        void Init();
        void Destroy();
        void Draw(const ml::vec3 &camPos, const std::vector<std::unique_ptr<ALight>> &lights, const ml::mat4 &projection, const ml::mat4 &view, const ml::mat4 &initTransform, bool enableRootMotion = true);
        void DrawSubModels(size_t nodeIndex, const ml::vec3 &camPos, const std::vector<std::unique_ptr<ALight>> &lights, const ml::mat4 &projection, const ml::mat4 &view, std::map<int, ml::mat4> &nodesTransform);
        
        std::vector<std::string> GetAnimationsName() const { return (animator.GetAnimationsName()); }
        void Play(const std::string &name, float animationSpeed = 1.0f) { animator.Play(name, animationSpeed); }
        bool CurrentAnimationEnded() const { return animator.CurrentAnimationEnded(); }
        std::string GetCurrentAnimation() const { return animator.GetCurrentAnimation(); }
        float GetCurrentAnimationDuration() const { return animator.CurrentAnimationDuration(); }

        std::map<int, NodeModel> &GetNodes() { return (nodes); }
        int GetRootIndex() { return (nodeIndex); }
        
    private:
        int DetermineRootNode(int node);
        void LoadMesh(const Glb::GltfData &data, size_t nodeIndex);
        void LoadAnimations(const Glb::GltfData &data);
        std::map<int, ml::mat4> CalculateNodeTransform(size_t nodeIndex, const ml::mat4 &parentTransform, bool enableRootMotion);

        int nodeIndex;
        std::map<int, NodeModel> nodes;

        std::vector<Mesh> meshes;
        Animator3D animator;

};