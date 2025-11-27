#include "Engine/WindowManager/WindowManager.hpp"
#include <cstdlib>

int main()
{
    WindowManager::InitWindow(EXECUTABLE_NAME);
    WindowManager::StartUpdateLoop(NULL);
    WindowManager::DestructWindowManager();

    return (EXIT_SUCCESS);
}