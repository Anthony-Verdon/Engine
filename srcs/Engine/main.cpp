#include "Engine/WindowManager/WindowManager.hpp"
#include <cstdlib>

int main()
{
    WindowManager::InitWindow("new game");
    WindowManager::StartUpdateLoop(NULL);
    WindowManager::DestructWindowManager();

    return (EXIT_SUCCESS);
}