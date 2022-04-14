cmake_minimum_required(VERSION 3.15)

if(UNIX)
  SET (CMAKE_EXE_LINKER_FLAGS      "-Wl,--no-as-needed -fuse-ld=lld -ldl")
  SET (CMAKE_CXX_FLAGS_DEBUG       "${CMAKE_CXX_FLAGS_DEBUG} -fstandalone-debug")
endif()

# Forcing colored outputs(mainly applies to Ninja builds)
option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()



set(CMAKE_CXX_STANDARD 17)

project("Noor")

set(src
  "src/init.cpp"
  "src/window.cpp"
  "src/log.cpp"
  "src/opengl_api.cpp"
  "src/opengl_buffers.cpp"
  )

add_library(Noor STATIC ${src})

target_link_libraries(Noor glad glfw spdlog glm)
target_include_directories(Noor PUBLIC
  "src"
  "deps/glad/include"
  "deps/glfw/include"
  "deps/spdlog/include"
  "deps/glm"
  )

set_target_properties(Noor PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(deps/glad)
add_subdirectory(deps/glfw)
add_subdirectory(deps/spdlog)
add_subdirectory(deps/glm)

add_subdirectory(sandbox)
