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

#if FULL_SCREEN == 0
constexpr int SMALL_WINDOW_WIDTH = 1280;
constexpr int SMALL_WINDOW_HEIGHT = 720;
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
    static void InitWindow(const std::string &name);
    static void DestructWindowManager();

    static void StartUpdateLoop(AProgram *program);
    static void StopUpdateLoop();
    static bool UpdateLoopRunning();

    static void SetClearColor(float r, float g, float b, float a);
    // @todo: if we want to avoid using OpenGL and/or GLFW #define
    // I should create my own #define for this arg
    static void EnableDepthTest();
    static void DisableDepthTest();
    static void EnableCullFace(GLenum cullFaceMode, GLenum frontFaceMode);
    static void DisableCullFace();
    static void EnableBlend(GLenum sfactor, GLenum dfactor);
    static void DisableBlend();

    static void SetWindowIcon(const std::string &path);

    // @todo: if we want to avoid using OpenGL and/or GLFW #define
    // I should create my own #define for input
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
    static float GetWindowAspectRatio() { return windowSize.x / windowSize.y; }

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