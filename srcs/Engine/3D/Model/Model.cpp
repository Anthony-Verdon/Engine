#include "Engine/3D/Model/Model.hpp"
#include "Engine/3D/ModelManager/ModelManager.hpp"

Model::Model()
{
}

Model::Model(const Glb::GltfData &data, size_t nodeIndex)
{
    LoadMesh(data, nodeIndex);
    LoadAnimations(data);
    this->nodeIndex = DetermineRootNode(nodeIndex);
    if (this->nodeIndex == -1)
        this->nodeIndex = nodeIndex;
    animator.Play("");
}

Model::~Model()
{
}

int Model::DetermineRootNode(int node)
{
    if (animator.IsNodeAnimated(node))
        return (node);

    for (size_t i = 0; i < nodes[node].children.size(); i++)
    {
        int possibleRootNode = DetermineRootNode(nodes[node].children[i]);
        if (possibleRootNode != -1)
            return (possibleRootNode);
    }

    return (-1);
}

void Model::LoadMesh(const Glb::GltfData &data, size_t nodeIndex)
{
    auto node = data.nodes[nodeIndex];

    if (node.mesh != -1)
        meshes.push_back({data, nodeIndex});

    nodes[nodeIndex] = {};
    nodes[nodeIndex].name = node.name;
    nodes[nodeIndex].transform = node.transform;
    for (size_t i = 0; i < node.children.size(); i++)
    {
        nodes[nodeIndex].children.push_back(node.children[i]);
        LoadMesh(data, node.children[i]);
    }
}

void Model::LoadAnimations(const Glb::GltfData &data)
{
    for (size_t i = 0; i < data.animations.size(); i++)
        animator.LoadAnimation(data.nodes, data.animations[i]);
}

void Model::Init()
{
    for (size_t i = 0; i < meshes.size(); i++)
        meshes[i].Init();
}

void Model::Destroy()
{
    for (size_t i = 0; i < meshes.size(); i++)
        meshes[i].Destroy();
}

void Model::Draw(const ml::vec3 &camPos, const std::vector<std::unique_ptr<ALight>> &lights, const ml::mat4 &projection, const ml::mat4 &view, const ml::mat4 &initTransform, bool enableRootMotion, const ml::vec3 &color)
{
    animator.Update();
    auto nodesTransform = CalculateNodeTransform(nodeIndex, initTransform, enableRootMotion);
    for (size_t i = 0; i < meshes.size(); i++)
        meshes[i].Draw(camPos, lights, projection, view, nodesTransform, color);
    DrawSubModels(nodeIndex, camPos, lights, projection, view, nodesTransform, color);
}

std::map<int, ml::mat4> Model::CalculateNodeTransform(size_t nodeIndex, const ml::mat4 &parentTransform, bool enableRootMotion)
{
    auto node = nodes[nodeIndex];

    ml::mat4 transform;
    if (animator.GetCurrentAnimation() == "")
        transform = node.transform;
    else
        transform = animator.GetNodeTransform(nodeIndex);

    if (!enableRootMotion)
    {
        transform[0][3] = 0;
        transform[1][3] = 0;
        transform[2][3] = 0;
    }

    transform = parentTransform * transform;
    std::map<int, ml::mat4> nodesTransform;
    nodesTransform[nodeIndex] = transform;

    for (size_t i = 0; i < node.children.size(); i++)
        nodesTransform.merge(CalculateNodeTransform(node.children[i], transform, true));

    return (nodesTransform);
}

void Model::DrawSubModels(size_t nodeIndex, const ml::vec3 &camPos, const std::vector<std::unique_ptr<ALight>> &lights, const ml::mat4 &projection, const ml::mat4 &view, std::map<int, ml::mat4> &nodesTransform, const ml::vec3 &color)
{
    auto node = nodes[nodeIndex];
    for (size_t i = 0; i < node.children.size(); i++)
        DrawSubModels(node.children[i], camPos, lights, projection, view, nodesTransform, color);
    for (size_t i = 0; i < node.models.size(); i++)
        ModelManager::Draw(node.models[i], camPos, lights, projection, view, nodesTransform[nodeIndex], true, color);
}
