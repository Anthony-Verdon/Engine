#include "Engine/WindowManager/WindowManager.hpp"
#include <cstdlib>

int main()
{
    WindowManager::InitWindow("new game", WINDOW_WIDTH, WINDOW_HEIGHT);
    WindowManager::StartUpdateLoop(NULL);
    WindowManager::DestructWindowManager();

    return (EXIT_SUCCESS);
}