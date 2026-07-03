workspace "DogEngine"
    architecture "x64"
    startproject"Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "DogEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "DogEngine/vendor/glad/include"
IncludeDir["ImGui"] = "DogEngine/vendor/imgui"
IncludeDir["glm"] = "DogEngine/vendor/glm"



include "DogEngine/vendor/GLFW"
include "DogEngine/vendor/glad" 
include "DogEngine/vendor/imgui"

project "DogEngine"
    location "DogEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    pchheader "gepch.h"
    pchsource "src/gepch.cpp"
    files
    {
        "src/**.h",
        "src/**.cpp",
        "src/DogEngine/**.h",
        "src/DogEngine/**.cpp",
        "%{prj.name}/vendor/glad/src/glad.c",
		"%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    includedirs
    {
        "src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
    }

    links{
        "GLFW",
        "GLAD",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "GE_PLATFORM_WINDOWS",
            "GE_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
            "_CRT_SECURE_NO_WARNINGS"
        }

        buildoptions { "/utf-8" }

        

    filter "files:**/glad.c"
        enablepch "Off"

    filter "configurations:Debug"
        defines "GE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "GE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "GE_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

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
        "DogEngine/vendor/spdlog/include",
        "DogEngine/vendor",
        "%{IncludeDir.glm}" 
    }

    links
    {
        "DogEngine"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "GE_PLATFORM_WINDOWS"
        }

        buildoptions { "/utf-8" }

    filter "configurations:Debug"
        defines "GE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "GE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "GE_DIST"
        runtime "Release"
        optimize "on"
