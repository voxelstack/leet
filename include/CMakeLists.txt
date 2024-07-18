add_library(leet INTERFACE)
target_include_directories(leet INTERFACE .)
target_precompile_headers(leet INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/leet.h)
