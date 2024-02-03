file(GLOB global_buffer_code_files "./*.cpp")
file(GLOB global_buffer_header_files "./*.hpp")

add_library(
    global_buffer_lib
    ${global_buffer_code_files}
    ${global_buffer_header_files}
)