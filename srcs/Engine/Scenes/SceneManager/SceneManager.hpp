#pragma once

#include "Engine/Scenes/AScene/AScene.hpp"

class SceneManager
{
  private:
    SceneManager() = delete;
    ~SceneManager() = delete;

    static std::unique_ptr<AScene> currentScene;

  public:
    static void LoadScene(std::unique_ptr<AScene> newScene);
    static void SwitchScene(std::unique_ptr<AScene> newScene);
    static void QuitScene();
    static void UpdateCurrentScene();
    static unsigned int GetCurrentSceneID();
};