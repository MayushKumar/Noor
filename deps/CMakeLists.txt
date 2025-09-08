add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(spdlog)
add_subdirectory(glm)


################################### ImGui #############################
set(imgui_src
"imgui/imconfig.h"
"imgui/imgui.cpp"
"imgui/imgui.h"
"imgui/imgui_demo.cpp"
"imgui/imgui_draw.cpp"
"imgui/imgui_internal.h"
"imgui/imgui_tables.cpp"
"imgui/imgui_widgets.cpp"
"imgui/imstb_rectpack.h"
"imgui/imstb_textedit.h"
"imgui/imstb_truetype.h"

"imgui/backends/imgui_impl_opengl3.h"
"imgui/backends/imgui_impl_opengl3.cpp"
"imgui/backends/imgui_impl_glfw.h"
"imgui/backends/imgui_impl_glfw.cpp"
)

add_library(imgui STATIC ${imgui_src})
target_include_directories(imgui PRIVATE "imgui" "imgui/backends")

set_target_properties(imgui PROPERTIES LINKER_LANGUAGE CXX)
