#pragma once

#include <vector>
#include "Engine/3D/Model/Model.hpp"

class ModelManager
{
  public:
    static Model &GetModel(int index) { return (models[index]); }
    static void AddModels(const std::vector<Model> &newModels) { models.insert(ModelManager::models.end(), newModels.begin(), newModels.end()); }
    static size_t GetNbModel() { return (models.size()); }

    static void Draw(int modelIndex, const ml::vec3 &camPos, const std::vector<std::unique_ptr<ALight>> &lights, const ml::mat4 &projection, const ml::mat4 &view, const ml::mat4 &initTransform, bool enableRootMotion = true) { models[modelIndex].Draw(camPos, lights, projection, view, initTransform, enableRootMotion); }

  private:
    ModelManager();
    ~ModelManager();

    static std::vector<Model> models;
};