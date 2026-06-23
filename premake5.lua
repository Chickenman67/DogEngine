workspace "GodEngine"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "GodEngine/vendor/GLFW/include"

include "GodEngine/vendor/GLFW"

project "GodEngine"
    location "GodEngine"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    pchheader "gepch.h"
    pchsource "src/gepch.cpp"
    files
    {
        "src/**.h",
        "src/**.cpp",
        "src/GodEngine/**.h",
        "src/GodEngine/**.cpp"
    }

    includedirs
    {
        "src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}"
    }

    links{
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "GE_PLATFORM_WINDOWS",
            "GE_BUILD_DLL"
        }

        buildoptions { "/utf-8" }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "GE_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "GE_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "GE_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "src",
        "GodEngine/vendor/spdlog/include"
    }

    links
    {
        "GodEngine"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "GE_PLATFORM_WINDOWS"
        }

        buildoptions { "/utf-8" }

    filter "configurations:Debug"
        defines "GE_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "GE_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "GE_DIST"
        optimize "On"
