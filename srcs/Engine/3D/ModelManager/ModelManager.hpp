#pragma once

#include <vector>
#include "Engine/3D/Model/Model.hpp"

class ModelManager
{
  public:
    static Model &GetModel(int index) { return (models[index]); }
    static void AddModels(const std::vector<Model> &newModels) { models.insert(ModelManager::models.end(), newModels.begin(), newModels.end()); }
    static size_t GetNbModel() { return (models.size()); }

    static void Draw(int modelIndex, const ml::vec3 &camPos, const std::vector<std::unique_ptr<ALight>> &lights, const ml::mat4 &projection, const ml::mat4 &view, const ml::mat4 &initTransform, bool enableRootMotion = true, const ml::vec3 &color = ml::vec3(1, 1, 1)) { models[modelIndex].Draw(camPos, lights, projection, view, initTransform, enableRootMotion, color); }

  private:
    ModelManager();
    ~ModelManager();

    static std::vector<Model> models;
};