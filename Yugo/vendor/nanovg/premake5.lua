project "nanovg"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "nanovg/fontstash.h",
        "nanovg/nanovg.h",
        "nanovg/nanovg_gl.h",
        "nanovg/nanovg_gl_utils.h",
        "nanovg/stb_image.h",
        "nanovg/stb_truetype.h",
        "nanovg/nanovg.c"
    }

    includedirs
    {
        "include"
    }
    
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"