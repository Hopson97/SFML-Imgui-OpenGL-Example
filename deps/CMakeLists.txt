add_library(imgui_impl
    imgui_impl/imgui_impl_opengl3.cpp
    imgui_impl/imgui_impl_sfml.cpp
) 

add_library(imgui
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
)

add_library(glad glad/glad.c)   
target_compile_options(glad PRIVATE -fPIE)		


add_library(stb stb/stb_impl.c)   
target_compile_options(stb PRIVATE -fPIE)	