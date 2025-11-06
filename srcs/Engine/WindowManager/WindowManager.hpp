#pragma once

#include <glad/glad.h>

#include "Engine/AProgram/AProgram.hpp"
#include "Matrix/Matrix.hpp"
#include <GLFW/glfw3.h>
#include <map>
#include <string>
#ifdef HOTRELOAD
#include <filesystem>
#endif

#if FULL_SCREEN
constexpr int WINDOW_WIDTH = 1920;
constexpr int WINDOW_HEIGHT = 1080;
#else
constexpr int WINDOW_WIDTH = 1080;
constexpr int WINDOW_HEIGHT = 720;
#endif

struct InputMode
{
    int mode;
    float time;
};

class WindowManager
{
  private:
    static GLFWwindow *window;
    static ml::vec2 windowSize;
    static ml::vec2 mousePosition;
    static std::map<int, InputMode> inputMap;
    static AProgram *program;
#ifdef HOTRELOAD
    static void *DLL;
    static std::filesystem::file_time_type DLLtimestamp;
#endif

    WindowManager() = delete;
    ~WindowManager() = delete;

  public:
    static void InitWindow(const std::string &name, unsigned int width, unsigned int height);
    static void StartUpdateLoop(AProgram *program);
    static void StopUpdateLoop();
    static void DestructWindowManager();

    static bool IsInputPressed(int input);
    static bool IsInputPressedOrMaintain(int input, float time = 0);
    static bool IsInputReleased(int input, float time = 0);
    static void SetInputAction(int input, int action)
    {
        inputMap[input] = {action, 0};
    }

    static void SetInputMode(int mode, int value);
    static int GetInputMode(int mode);

    static GLFWwindow *GetWindow();

    static ml::vec2 GetWindowSize();
    static unsigned int GetWindowWidth();
    static unsigned int GetWindowHeight();

    static ml::vec2 GetMousePosition();
    static void SetMousePosition(double xPos, double yPos);

    static void SetUserPointer(void *ptr);

#ifdef HOTRELOAD
    static void SwapDLL();
    static std::string ConvertTimeStampToString(const std::filesystem::file_time_type &timestamp);
    static void *LoadDLL(const std::string &path);
    static void *LoadFunctionFromDLL(void *DLL, const std::string &func);
    static bool UnloadDLL(void *DLL);
#endif

    static void SetCharCallback(void (*func)(GLFWwindow *window, unsigned int character));
    static void SetScrollCallback(void (*func)(GLFWwindow *window, double xOffset, double yOffset));
};