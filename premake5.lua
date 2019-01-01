workspace "Scripter"
  configurations { "Debug", "Release" }

project "Scripter"
  kind "SharedLib"
  
  language "C++"
  cppdialect "C++17"

  targetdir "bin/%{cfg.buildcfg}"
  objdir "bin/%{cfg.buildcfg}/obj/%{prj.name}"

  files { "include/scripter/**.h", "src/scripter/**.cpp" }

  filter "system:linux"
    toolset "clang"
    
    includedirs { "src/", "include/", "vendor/v8/include", "vendor/spdlog/include" }
    libdirs { "vendor/v8/libs" }
    
    links { "v8_monolith", "pthread" }

    buildoptions { "-Wall", "-Wextra", "-Wno-unused-parameter", "-Wno-unused-result"}

    defines { "SCRIPTER_PLATFORM_LINUX" }

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"

project "TestProgram"
  kind "ConsoleApp"
  
  language "C++"
  cppdialect "C++17"

  targetdir "bin/%{cfg.buildcfg}"
  objdir "bin/%{cfg.buildcfg}/obj/%{prj.name}"

  files { "src/main.cpp" }

  filter "system:linux"
    toolset "clang"
    includedirs { "include/", "vendor/v8/include", "vendor/spdlog/include" }
    links { "Scripter", "dl" }
    buildoptions { "-Wall", "-Wextra", "-Wno-unused-parameter", "-Wno-unused-result"}
    defines { "SCRIPTER_PLATFORM_LINUX" }

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"

project "Test"
  kind "SharedLib"
  
  language "C++"
  cppdialect "C++17"

  targetdir "bin/%{cfg.buildcfg}"
  objdir "bin/%{cfg.buildcfg}/obj/%{prj.name}"

  files { "src/test/**.h", "src/test/**.cpp" }

  filter "system:linux"
    toolset "clang"
    
    includedirs { "include/", "vendor/v8/include", "vendor/spdlog/include" }
    buildoptions { "-Wall", "-Wextra", "-Wno-unused-parameter", "-Wno-unused-result"}

    defines { "SCRIPTER_PLATFORM_LINUX" }

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"

if _ACTION == "clean" then
  os.rmdir("./bin");
end

