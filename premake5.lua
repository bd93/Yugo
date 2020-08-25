workspace "Yugo"
    architecture "x64"
    startproject "Playground"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Yugo/vendor/GLFW/include"
IncludeDir["Glad"] = "Yugo/vendor/Glad/include"
IncludeDir["Assimp"] = "Yugo/vendor/Assimp/include"
IncludeDir["ImGui"] = "Yugo/vendor/imgui"
IncludeDir["ImGuizmo"] = "Yugo/vendor/ImGuizmo"
IncludeDir["glm"] = "Yugo/vendor/glm"
IncludeDir["entt"] = "Yugo/vendor/entt"
IncludeDir["stb_image"] = "Yugo/vendor/stb_image"
IncludeDir["json"] = "Yugo/vendor/json"

group "Dependencies"
    include "Yugo/vendor/GLFW"
    include "Yugo/vendor/GLad"
    include "Yugo/vendor/imgui"
    include "Yugo/vendor/ImGuizmo"

group ""

project "Yugo"
    location "Yugo"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "Yugo/src/pch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/entt/**.hpp",
        "%{prj.name}/vendor/json/**.hpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.Assimp}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.json}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "ImGuizmo",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "YU_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "YU_DEBUG"
        runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines "YU_RELEASE"
        runtime "Release"
        optimize "On"
    
    filter "configurations:Dist"
        defines "YU_DIST"
        runtime "Release"
        optimize "On"

project "Playground"
        location "Playground"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "On"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }
    
        includedirs
        {
            "Yugo/src",
            "Yugo/vendor",
            "%{IncludeDir.GLFW}",
            "%{IncludeDir.Glad}",
            "%{IncludeDir.Assimp}",
            "%{IncludeDir.ImGui}",
            "%{IncludeDir.ImGuizmo}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.entt}",
            "%{IncludeDir.stb_image}",
            "%{IncludeDir.json}"
        }

        libdirs
        {
            "Yugo/vendor/Assimp/lib"
        }

        links
        {
            "Yugo",
            "assimp-vc142-mtd.lib"
        }
    
        filter "system:windows"
            systemversion "latest"
        
        filter "configurations:Debug"
            defines "YU_DEBUG"
            runtime "Debug"
            symbols "On"
        
        filter "configurations:Release"
            defines "YU_RELEASE"
            runtime "Release"
            optimize "On"
        
        filter "configurations:Dist"
            defines "YU_DIST"
            runtime "Release"
            optimize "On"

project "GameLogic"
        location "GameLogic"
        kind "SharedLib"
        language "C++"
        cppdialect "C++17"
        staticruntime "On"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }
    
        includedirs
        {
            "Yugo/src",
            "Yugo/vendor",
            "%{IncludeDir.GLFW}",
            "%{IncludeDir.Glad}",
            "%{IncludeDir.Assimp}",
            "%{IncludeDir.ImGui}",
            "%{IncludeDir.ImGuizmo}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.entt}",
            "%{IncludeDir.stb_image}",
            "%{IncludeDir.json}"
        }

        libdirs
        {
            "Yugo/vendor/Assimp/lib"
        }

        links
        {
            "Yugo",
            "assimp-vc142-mtd.lib"
        }
    
        filter "system:windows"
            systemversion "latest"
        
        filter "configurations:Debug"
            defines "YU_DEBUG"
            runtime "Debug"
            symbols "On"
        
        filter "configurations:Release"
            defines "YU_RELEASE"
            runtime "Release"
            optimize "On"
        
        filter "configurations:Dist"
            defines "YU_DIST"
            runtime "Release"
            optimize "On"