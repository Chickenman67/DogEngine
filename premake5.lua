workspace "GodEngine"
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
IncludeDir["GLFW"] = "GodEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "GodEngine/vendor/glad/include"
IncludeDir["ImGui"] = "GodEngine/vendor/imgui"
IncludeDir["glm"] = "GodEngine/vendor/glm"



include "GodEngine/vendor/GLFW"
include "GodEngine/vendor/glad" 
include "GodEngine/vendor/imgui"

project "GodEngine"
    location "GodEngine"
    kind "SharedLib"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    pchheader "gepch.h"
    pchsource "src/gepch.cpp"
    files
    {
        "src/**.h",
        "src/**.cpp",
        "src/GodEngine/**.h",
        "src/GodEngine/**.cpp",
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
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "GE_PLATFORM_WINDOWS",
            "GE_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
            "_CRT_SECURE_NO_WARNINGS"
        }

        buildoptions { "/utf-8" }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
        }

    filter "files:**/glad.c"
        enablepch "Off"

    filter "configurations:Debug"
        defines "GE_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "GE_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "GE_DIST"
        runtime "Release"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"

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
        "GodEngine/vendor/spdlog/include",
        "%{IncludeDir.glm}" 
    }

    links
    {
        "GodEngine"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "GE_PLATFORM_WINDOWS"
        }

        buildoptions { "/utf-8" }

    filter "configurations:Debug"
        defines "GE_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "GE_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "GE_DIST"
        runtime "Release"
        optimize "On"
