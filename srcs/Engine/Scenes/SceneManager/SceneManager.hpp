#pragma once

#include "Engine/Scenes/AScene/AScene.hpp"

class WindowManager;

class SceneManager
{
  private:
    SceneManager() = delete;
    ~SceneManager() = delete;

    static std::unique_ptr<AScene> currentScene;
    static std::unique_ptr<AScene> futureScene;

    static void SwitchScene();

    friend WindowManager;

  public:
    static void LoadScene(std::unique_ptr<AScene> newScene);
    static void SwitchScene(std::unique_ptr<AScene> newScene);
    static void QuitScene();
    static void UpdateCurrentScene();
    static std::unique_ptr<AScene> &GetCurrentScene() { return currentScene; }
    static unsigned int GetCurrentSceneID();
};