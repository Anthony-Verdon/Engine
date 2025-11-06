#pragma once

#include <vector>
#include "Engine/3D/Model/Model.hpp"

class ModelManager
{
  public:
    static Model &GetModel(int index) { return (models[index]); }
    static void AddModels(const std::vector<Model> &newModels) { models.insert(ModelManager::models.end(), newModels.begin(), newModels.end()); }
    static size_t GetNbModel() { return (models.size()); }

    // TODO: faire une fonction Draw qui prend les param de draw + l'index du modele

  private:
    ModelManager();
    ~ModelManager();

    static std::vector<Model> models;
};