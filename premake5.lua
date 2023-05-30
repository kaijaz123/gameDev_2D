workspace "workspace"
    architecture "x86_64"

    configurations {
        "Debug",
        "Release"
    }


project "gui"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("bin/x86_64/")
    objdir ("bin-int/x86_64/")

    files
    {
        "gui/src/**.h",
        "gui/src/**.cpp"
    }

    includedirs --include the all the source code of library
    {
        "gui/vendor/SDL2/x86_64-w64-mingw32/include",
        "gui/vendor/SDL2_image/x86_64-w64-mingw32/include",
        -- "gui/vendor/spdlog/include"
    }

    libdirs -- add the extra lib (specify the path here then the name in links) 
    -- note: its equal to -L
    {
        "gui/vendor/SDL2/x86_64-w64-mingw32/lib",
        "gui/vendor/SDL2_image/x86_64-w64-mingw32/lib"
    }

    -- Note: this one is used on macbook but not windows, not sure why yet
    -- linkoptions --linkoptions (past to linker so it knows where to link to the path, frameworks path) 
    -- -- note: its equal to the args -F
    -- {
    --     "-F /System/Library/Frameworks",
    --     "-F /Library/Frameworks"
    --     -- "-llibglfw.3.3.dylib" *another option for links (section below) glfw.3.3
    -- }

    links --links (the actual project or library/framework name from either libdirs or linkoptions) 
    -- note: on mac its equal to the args -framework
    -- note: on windows its equal to the args -l
    {
        "SDL2",
        "SDL2_image",
    }

    postbuildcommands
    {
        "cp 'gui/vendor/SDL2/x86_64-w64-mingw32/bin/SDL2.dll' 'bin/x86_64/'",
        "cp 'gui/vendor/SDL2_image/x86_64-w64-mingw32/bin/SDL2_image.dll' 'bin/x86_64/'",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        symbols "off"
        optimize "on"
    
    -- note : on windows remember to copy and paste the dll file to the exact same path of the exe path


