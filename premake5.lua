workspace "RePlex"
  configurations {"Debug", "Release"}
  -- Use C++ as the target language for all builds
  language "C++"
  targetdir "bin/%{cfg.buildcfg}"

  -- Get that C++14 goodness
  cppdialect "C++14"

  filter "configurations:Debug"
    -- Add the preprocessor definition DEBUG to debug builds
    defines { "DEBUG" }
    -- Ensure symbols are bundled with debug builds
    symbols "On"

  filter "configurations:Release"
    -- Add the preprocessor definition RELEASE to debug builds
    defines { "RELEASE" }
    -- Turn on compiler optimizations for release builds
    optimize "On"

  -- RePlex Library
  project "RePlex"
    kind "SharedLib"
    -- recursively glob .h and .cpp files in the lib directory
    files { "lib/**.h", "lib/**.cpp" }

  -- RePlex Runtime
  project "RePlexRuntime"
    kind "ConsoleApp"
    -- recursively glob .h and .cpp files in the runtime directory
    files { "runtime/**.h", "runtime/**.cpp" }
    -- link the RePlexLib library at runtime
    links { "RePlex" }
    includedirs { "lib/pub" }