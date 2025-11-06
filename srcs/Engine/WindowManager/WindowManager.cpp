#include "Engine/WindowManager/WindowManager.hpp"
#include "Engine/Time/Time.hpp"
#include "Engine/defines.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdexcept>
#include "Engine/macros.hpp"
#ifdef HOTRELOAD
#include <dlfcn.h>
#include <chrono>
#define HOTRELOAD_LIB_DIRECTORY "./GameplayDLL/"
#define DLL_NAME "./lib" EXECUTABLE_NAME
#endif

GLFWwindow *WindowManager::window = NULL;
ml::vec2 WindowManager::mousePosition = ml::vec2(0, 0);
ml::vec2 WindowManager::windowSize = ml::vec2(0, 0);
std::map<int, InputMode> WindowManager::inputMap;
AProgram *WindowManager::program = NULL;
#ifdef HOTRELOAD
void *WindowManager::DLL = NULL;
std::filesystem::file_time_type WindowManager::DLLtimestamp;
#endif

void mouse_position_callback(GLFWwindow *window, double xPos, double yPos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void WindowManager::InitWindow(const std::string &name, unsigned int width, unsigned int height)
{
    if (glfwInit() == GL_FALSE)
        throw(std::runtime_error("INIT_GLFW::INITIALIZATION_FAILED"));

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if FULL_SCREEN
    window = glfwCreateWindow(width, height, name.c_str(), glfwGetPrimaryMonitor(), NULL);
#else
    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
#endif

    if (!window)
        throw(std::runtime_error("INIT_WINDOW::INITIALIZATION_FAILED"));
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw(std::runtime_error("INIT_OPENGL::INITIALIZATION_FAILED"));

    int viewPortWidth, viewportHeight;
    glfwGetFramebufferSize(window, &viewPortWidth, &viewportHeight);
    glViewport(0, 0, viewPortWidth, viewportHeight);

    // region to move in specific function
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    // cull face enabled make openGL draw only on one side
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    // glFrontFace(GL_CW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    // end region

    glfwSetCursorPosCallback(window, mouse_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);

    windowSize = ml::vec2(width, height);
}

void WindowManager::DestructWindowManager()
{
#ifdef HOTRELOAD
    if (DLL)
        UnloadDLL(DLL);
    if (std::filesystem::exists(HOTRELOAD_LIB_DIRECTORY))
        std::filesystem::remove_all(HOTRELOAD_LIB_DIRECTORY);
#endif

    glfwTerminate();
}

void WindowManager::StartUpdateLoop(AProgram *inProgram)
{
#ifdef HOTRELOAD
    if (!inProgram)
        SwapDLL();
    else
        program = inProgram;
#else
    program = inProgram;
#endif

    CHECK_AND_RETURN_VOID(program, "program pointer is NULL");

    while (!glfwWindowShouldClose(window))
    {
#ifdef HOTRELOAD
        if (DLLtimestamp != std::filesystem::last_write_time(DLL_NAME ".so"))
            SwapDLL();
#endif
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Time::updateTime();

        program->Run();

        for (auto it = inputMap.begin(); it != inputMap.end(); it++)
        {
            if (it->second.mode == GLFW_PRESS)
                it->second.mode = GLFW_REPEAT;
            it->second.time += Time::getDeltaTime();
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void WindowManager::StopUpdateLoop()
{
    glfwSetWindowShouldClose(window, true);
}

bool WindowManager::IsInputPressed(int input)
{
    if (inputMap.find(input) == inputMap.end())
    {
        inputMap[input] = {GLFW_RELEASE, 0};
        return (false);
    }

    return (inputMap[input].mode == GLFW_PRESS);
}

bool WindowManager::IsInputPressedOrMaintain(int input, float time)
{
    if (inputMap.find(input) == inputMap.end())
    {
        inputMap[input] = {GLFW_RELEASE, 0};
        return (false);
    }

    return ((inputMap[input].mode == GLFW_PRESS || inputMap[input].mode == GLFW_REPEAT) && inputMap[input].time >= time);
}

bool WindowManager::IsInputReleased(int input, float time)
{
    if (inputMap.find(input) == inputMap.end())
    {
        inputMap[input] = {GLFW_RELEASE, 0};
        return (true);
    }

    return (inputMap[input].mode == GLFW_RELEASE && inputMap[input].time >= time);
}

void WindowManager::SetInputMode(int mode, int value)
{
    glfwSetInputMode(window, mode, value);
}

int WindowManager::GetInputMode(int mode)
{
    return (glfwGetInputMode(window, mode));
}

GLFWwindow *WindowManager::GetWindow()
{
    return (window);
}

ml::vec2 WindowManager::GetWindowSize()
{
    return (windowSize);
}

unsigned int WindowManager::GetWindowWidth()
{
    return (windowSize.x);
}

unsigned int WindowManager::GetWindowHeight()
{
    return (windowSize.y);
}

void WindowManager::SetMousePosition(double xPos, double yPos)
{
    mousePosition = ml::vec2(xPos, yPos);
}

ml::vec2 WindowManager::GetMousePosition()
{
    return (mousePosition);
}

void WindowManager::SetUserPointer(void *ptr)
{
    glfwSetWindowUserPointer(window, ptr);
}

#ifdef HOTRELOAD
void WindowManager::SwapDLL()
{
    if (!std::filesystem::exists(HOTRELOAD_LIB_DIRECTORY))
        std::filesystem::create_directory(HOTRELOAD_LIB_DIRECTORY);

    if (DLLtimestamp == std::filesystem::last_write_time(DLL_NAME ".so"))
        return;

    DLLtimestamp = std::filesystem::last_write_time(DLL_NAME ".so");
    std::string copyname = HOTRELOAD_LIB_DIRECTORY DLL_NAME "-" + ConvertTimeStampToString(DLLtimestamp) + ".so";
    if (!std::filesystem::exists(copyname))
        std::filesystem::copy(DLL_NAME ".so", copyname);
    void *newDLL = LoadDLL(copyname);
    CHECK_AND_RETURN_VOID(newDLL, "failed to load DLL");

    auto create = (AProgram * (*)(AProgramState *)) LoadFunctionFromDLL(newDLL, "create");
    if (!create)
        std::cerr << "failed to load \"AProgram *create(AProgramState *)\" function" << std::endl;

    auto destroy = (void (*)(AProgram *))LoadFunctionFromDLL(newDLL, "destroy");
    if (!destroy)
        std::cerr << "failed to load \"void destroy(AProgram *)\" function" << std::endl;

    if (!create || !destroy)
    {
        UnloadDLL(newDLL);
        return;
    }

    AProgramState *state = NULL;
    if (DLL)
    {
        auto save = (AProgramState * (*)()) LoadFunctionFromDLL(DLL, "save");
        if (!save)
        {
            std::cerr << "failed to load \"AProgramState *save()\" function" << std::endl;
            UnloadDLL(newDLL);
            return;
        }

        state = save();
    }

    AProgram *newProgram = create(state);
    if (!newProgram)
    {
        UnloadDLL(newDLL);
        return;
    }
    else
    {
        destroy(program);
        program = newProgram;
        program->Init();
    }

    if (DLL)
        UnloadDLL(DLL);
    DLL = newDLL;
}

std::string WindowManager::ConvertTimeStampToString(const std::filesystem::file_time_type &timestamp)
{
    auto sctp = std::chrono::clock_cast<std::chrono::system_clock>(timestamp);
    std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);
    std::tm *tm = std::localtime(&cftime);
    std::ostringstream oss;
    // example: 2025-09-10-23-13-59
    oss << std::put_time(tm, "%Y-%m-%d-%H-%M-%S");
    return (oss.str());
}

void *WindowManager::LoadDLL(const std::string &path)
{
    std::string pathModif = "./" + path;
    CHECK_AND_RETURN(std::filesystem::exists(pathModif), NULL, pathModif << " not found");

    void *lib = dlopen(pathModif.c_str(), RTLD_NOW);
    char *errstr = dlerror();
    CHECK(!errstr, "A dynamic linking error occurred: " << errstr);
    CHECK(lib, "failed to load " << pathModif);

    return (lib);
}

void *WindowManager::LoadFunctionFromDLL(void *DLL, const std::string &func)
{
    CHECK_AND_RETURN(DLL, NULL, "no DLL supplied");

    void *proc = dlsym(DLL, func.c_str());
    CHECK_AND_RETURN(proc, NULL, "fail to load function " << func << "from DLL");

    return (proc);
}

bool WindowManager::UnloadDLL(void *DLL)
{
    CHECK_AND_RETURN(DLL, false, "no DLL supplied");

    int freeResult = dlclose(DLL);
    CHECK_AND_RETURN((freeResult == 0), false, "failed to close DLL (errorcode: " << freeResult << ")");

    return (freeResult);
}
#endif

void WindowManager::SetCharCallback(void (*func)(GLFWwindow *window, unsigned int character))
{
    glfwSetCharCallback(window, func);
}

void WindowManager::SetScrollCallback(void (*func)(GLFWwindow *window, double xOffset, double yOffset))
{
    glfwSetScrollCallback(window, func);
}

void mouse_position_callback(GLFWwindow *window, double xPos, double yPos)
{
    (void)window;
    WindowManager::SetMousePosition(xPos, yPos);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    (void)window;
    (void)mods;

    WindowManager::SetInputAction(button, action);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void)window;
    (void)scancode;
    (void)mods;

    if (action == GLFW_REPEAT)
        return;

    WindowManager::SetInputAction(key, action);
}