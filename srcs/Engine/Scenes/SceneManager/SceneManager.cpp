#include "Engine/Scenes/SceneManager/SceneManager.hpp"
#include "Engine/macros.hpp"

std::unique_ptr<AScene> SceneManager::currentScene = NULL;
std::unique_ptr<AScene> SceneManager::futureScene = NULL;

void SceneManager::LoadScene(std::unique_ptr<AScene> newScene)
{
    CHECK_AND_RETURN_VOID((currentScene == NULL), "a scene is already loaded, use SwitchScene");

    SwitchScene(std::move(newScene));
}

void SceneManager::SwitchScene(std::unique_ptr<AScene> newScene)
{
    CHECK_AND_RETURN_VOID((newScene != NULL), "new scene shouldn't be NULL");

    if (newScene)
        futureScene = std::move(newScene);
}

void SceneManager::SwitchScene()
{
    if (!futureScene)
        return;

    if (currentScene)
        currentScene->Quit();
    currentScene = std::move(futureScene);
    futureScene = NULL;
    currentScene->Load();
}

void SceneManager::QuitScene()
{
    CHECK_AND_RETURN_VOID((currentScene != NULL), "no scene loaded");

    currentScene->Quit();
    currentScene = NULL;
}

void SceneManager::UpdateCurrentScene()
{
    CHECK_AND_RETURN_VOID((currentScene != NULL), "no scene loaded");

    currentScene->Run();
}

unsigned int SceneManager::GetCurrentSceneID()
{
    CHECK_AND_RETURN((currentScene != NULL), 0, "no scene loaded");

    return (currentScene->GetID());
}