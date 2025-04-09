set_languages("cxx17")

set_toolchains("clang")
set_warnings("allextra", "error")
set_optimize("fastest")
set_symbols("debug")

add_requires("glfw")
add_requires("stb")
add_requires("glad")
add_requires("box2d")
add_requires("joltphysics")

namespace("GlbParser", function ()
    includes("submodules/GlbParser")
end)

target("Engine")
    set_targetdir("./")
    set_kind("static")
    add_files("srcs/**.cpp")
    add_includedirs("srcs", {public = true})
    add_packages("glfw")
    add_packages("stb")
    add_packages("glad", {public = true})
    add_packages("box2d")
    add_packages("joltphysics", {public = true})
    add_deps("GlbParser::GlbParser")
