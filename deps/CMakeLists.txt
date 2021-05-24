
add_library(cute_headers
    cute_headers/cute_impl.c
)

add_library(glad glad/glad.c)   
target_compile_options(glad PRIVATE -fPIE)		

add_library(stb stb/stb_impl.c)   
target_compile_options(stb PRIVATE -fPIE)	