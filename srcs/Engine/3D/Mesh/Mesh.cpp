#include "Engine/3D/Mesh/Mesh.hpp"
#include "Engine/RessourceManager/RessourceManager.hpp"
#include "Toolbox.hpp"
#include <cmath>
#include <glad/glad.h>

bool Mesh::shaderInitialized = false;

Mesh::Mesh(const Glb::GltfData &data, size_t nodeIndex)
{
    if (!shaderInitialized)
    {
        RessourceManager::AddShader("mesh_shader", PATH_TO_ENGINE "shaders/meshShader.vs", PATH_TO_ENGINE "shaders/meshShader.fs");
        shaderInitialized = true;
    }

    this->nodeIndex = nodeIndex;
    auto node = data.nodes[nodeIndex];
    auto mesh = data.meshes[node.mesh];
    name = mesh.name;
    primitives = mesh.primitives;
    for (size_t i = 0; i < primitives.size(); i++)
    {
        int materialIndex = primitives[i].material;
        if (materialIndex == -1)
            continue;

        auto material = data.materials[materialIndex];
        materials[materialIndex] = material;

        int imageIndex = material.pbr.baseColorTexture;
        if (imageIndex == -1)
            continue;

        auto image = data.images[imageIndex];
        baseColorTextures[imageIndex] = image.name;
        if (!RessourceManager::TextureExist(image.name))
            RessourceManager::AddTexture(image.name, image.buffer, image.bufferLength);
    }
    if (node.skin != -1)
    {
        auto skin = data.skins[node.skin];
        joints = skin.joints;
    }
}

Mesh::~Mesh()
{
}

void Mesh::Init()
{

    for (size_t i = 0; i < primitives.size(); i++)
    {
        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(Glb::Vertex) * primitives[i].vertices.size(), primitives[i].vertices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * primitives[i].indices.size(), primitives[i].indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, Glb::nbFloatPerPosition, GL_FLOAT, GL_FALSE, sizeof(Glb::Vertex), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, Glb::nbFloatPerTexCoord, GL_FLOAT, GL_FALSE, sizeof(Glb::Vertex), (void *)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, Glb::nbFloatPerNormal, GL_FLOAT, GL_FALSE, sizeof(Glb::Vertex), (void *)(sizeof(float) * 5));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, Glb::nbFloatPerJoint, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(Glb::Vertex), (void *)(sizeof(float) * 8));
        glEnableVertexAttribArray(3);

        glVertexAttribPointer(4, Glb::nbFloatPerWeight, GL_FLOAT, GL_FALSE, sizeof(Glb::Vertex), (void *)(sizeof(float) * 8 + sizeof(uint16_t) * 4));
        glEnableVertexAttribArray(4);

        VAOs.push_back(VAO);
        VBOs.push_back(VBO);
        EBOs.push_back(EBO);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Destroy()
{
    glDeleteVertexArrays(VAOs.size(), VAOs.data());
    glDeleteBuffers(VBOs.size(), VBOs.data());
    glDeleteBuffers(EBOs.size(), EBOs.data());
    VAOs.clear();
    VBOs.clear();
    EBOs.clear();

    shaderInitialized = false;
}

void Mesh::Draw(const ml::vec3 &camPos, const std::vector<std::unique_ptr<ALight>> &lights, const ml::mat4 &projection, const ml::mat4 &view, std::map<int, ml::mat4> &nodesTransform)
{
    auto shader = RessourceManager::GetShader("mesh_shader");
    shader->use();
    // vs
    shader->setMat4("uProjection", projection);
    shader->setMat4("uView", view);
    shader->setMat4("uModel", nodesTransform[nodeIndex]);
    shader->setInt("uUseJoints", (joints.size() != 0));
    for (size_t i = 0; i < joints.size(); i++)
        shader->setMat4("uJointMat[" + std::to_string(i) + "]", nodesTransform[joints[i].nodeIndex] * joints[i].inverseBindMatrix);

    // fs
    shader->setVec3("uCamPos", camPos);
    int nbPointLight = 0;
    int nbDirectionalLight = 0;
    int nbSpotLight = 0;
    for (size_t i = 0; i < lights.size(); i++)
    {
        switch (lights[i]->type)
        {
        case POINT_LIGHT: {
            auto light = dynamic_cast<PointLight *>(lights[i].get());
            shader->setVec3("uPointLights[" + std::to_string(nbPointLight) + "].position", light->position);
            shader->setVec3("uPointLights[" + std::to_string(nbPointLight) + "].color", light->color);
            shader->setFloat("uPointLights[" + std::to_string(nbPointLight) + "].intensity", light->intensity);
            nbPointLight++;
            break;
        }
        case DIRECTIONAL_LIGHT: {
            auto light = dynamic_cast<DirectionalLight *>(lights[i].get());
            shader->setVec3("uDirectionalLights[" + std::to_string(nbDirectionalLight) + "].direction", light->direction);
            shader->setVec3("uDirectionalLights[" + std::to_string(nbDirectionalLight) + "].color", light->color);
            shader->setFloat("uDirectionalLights[" + std::to_string(nbDirectionalLight) + "].intensity", light->intensity);
            nbDirectionalLight++;
            break;
        }
        case SPOT_LIGHT: {
            auto light = dynamic_cast<SpotLight *>(lights[i].get());
            shader->setVec3("uSpotLights[" + std::to_string(nbSpotLight) + "].position", light->position);
            shader->setVec3("uSpotLights[" + std::to_string(nbSpotLight) + "].direction", light->direction);
            shader->setFloat("uSpotLights[" + std::to_string(nbSpotLight) + "].cutOff", cosf(ml::radians(light->cutOff)));
            shader->setFloat("uSpotLights[" + std::to_string(nbSpotLight) + "].outerCutOff", cosf(ml::radians(light->outerCutOff)));
            shader->setVec3("uSpotLights[" + std::to_string(nbSpotLight) + "].color", light->color);
            shader->setFloat("uSpotLights[" + std::to_string(nbSpotLight) + "].intensity", light->intensity);
            nbSpotLight++;
            break;
        }
        default:
            break;
        }
    }

    shader->setInt("uNbPointLight", nbPointLight);
    shader->setInt("uNbDirectionalLight", nbDirectionalLight);
    shader->setInt("uNbSpotLight", nbSpotLight);

    for (size_t i = 0; i < materials.size(); i++)
    {
        shader->setVec4("uMaterials[" + std::to_string(i) + "].baseColorFactor", materials[i].pbr.baseColorFactor);
        shader->setVec3("uMaterials[" + std::to_string(i) + "].emissiveFactor", materials[i].emissiveFactor);
        shader->setFloat("uMaterials[" + std::to_string(i) + "].metallicFactor", materials[i].pbr.metallicFactor);
        shader->setFloat("uMaterials[" + std::to_string(i) + "].roughnessFactor", materials[i].pbr.roughnessFactor);
        shader->setFloat("uMaterials[" + std::to_string(i) + "].ambientOcclusion", 1.0);
    }

    for (size_t i = 0; i < primitives.size(); i++)
    {
        glBindVertexArray(VAOs[i]);
        int materialIndex = primitives[i].material;
        if (materialIndex != -1)
        {
            shader->setInt("uMaterialIndex", materialIndex);
            int baseColorTextureIndex = materials[materialIndex].pbr.baseColorTexture;
            bool useBaseColorTexture = (baseColorTextureIndex != -1);
            shader->setInt("uUseBaseColorTexture", useBaseColorTexture);
            if (useBaseColorTexture)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, RessourceManager::GetTexture(baseColorTextures[baseColorTextureIndex])->getID());
            }
        }

        glDrawElements(GL_TRIANGLES, primitives[i].indices.size(), GL_UNSIGNED_SHORT, nullptr);
    }
}