set_languages("cxx20")

set_toolchains("clang")
set_warnings("allextra", "error")
set_optimize("fastest")
set_symbols("debug")
add_cxxflags("-fPIC")

add_requires("glfw")
add_requires("stb")
add_requires("glad")
add_requires("box2d  3.0.0")
add_requires("joltphysics", {configs = {rtti = true}})
add_requires("freetype")

option("HOTRELOAD")
    set_showmenu(true)
    set_description("Enable Hotreload")
    set_default(false)
    set_category("Features")
option_end()

function split(inputstr, sep)
  local t = {}
  for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
    table.insert(t, str)
  end
  return t
end

targetDir = "./"
targetName = "Exe"

if has_config("PATH_TO_ENGINE") then
    add_defines("PATH_TO_ENGINE=\"" .. get_config("PATH_TO_ENGINE") .. "\"");
    if has_config("HOTRELOAD") then
        folders = split(get_config("PATH_TO_ENGINE"), "/")
        for _ in pairs(folders) do
            targetDir = targetDir .. "../"
        end
    end
else
    add_defines("PATH_TO_ENGINE=\"\"");
end

if has_config("HOTRELOAD") then
    if has_config("TARGET_NAME") then
        targetName = get_config("TARGET_NAME")
    end
    add_defines("HOTRELOAD")
    add_defines("EXECUTABLE_NAME=\"" .. targetName .. "\"");
end

if has_config("FULL_SCREEN") then
    add_defines("FULL_SCREEN=" .. get_config("FULL_SCREEN"));
else
    add_defines("FULL_SCREEN=1");
end

if has_config("DEBUG_DRAW_PHYSIC_3D") then
    add_defines("DEBUG_DRAW_PHYSIC_3D=" .. get_config("DEBUG_DRAW_PHYSIC_3D"));
else
    add_defines("DEBUG_DRAW_PHYSIC_3D=0");
end

namespace("GlbParser", function ()
    includes("submodules/GlbParser")
end)

target("Exe")
    if has_config("HOTRELOAD") then 
        set_targetdir(targetDir)
        set_kind("binary")
        add_files("srcs/Engine/main.cpp")
        add_deps("Engine")
        set_basename(targetName)
    end

target("Engine")
    set_targetdir("./")
    set_kind("shared")
    add_files("srcs/**.cpp")
    remove_files("srcs/Engine/main.cpp")
    add_includedirs("srcs", {public = true})
    add_packages("glfw")
    add_packages("stb")
    add_packages("glad", {public = true})
    add_packages("box2d")
    add_packages("joltphysics", {public = true})
    add_packages("freetype")
    add_deps("GlbParser::GlbParser")
