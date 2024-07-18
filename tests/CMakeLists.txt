file(GLOB_RECURSE LEET_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

foreach(PATH ${LEET_TESTS})
    file(RELATIVE_PATH NAME ${CMAKE_CURRENT_SOURCE_DIR} ${PATH})
    string(REPLACE ".c" "" NAME ${NAME})
    string(REPLACE "/" "_" NAME ${NAME})

    add_executable("test-${NAME}" ${PATH})

    # Test macros execute functions that are defined at the end of the file.
    # Test functions always return int and have no arguments.
    target_compile_options("test-${NAME}" PRIVATE "-Wno-implicit-function-declaration")

    target_link_libraries("test-${NAME}" PRIVATE leet)
    add_test(
        "build-${NAME}"
        "${CMAKE_COMMAND}"
        --build "${CMAKE_BINARY_DIR}"
        --config "$<CONFIG>"
        --target "test-${NAME}"
    )
    set_tests_properties("build-${NAME}" PROPERTIES FIXTURES_SETUP "build-${NAME}")
    add_test(NAME "test-${NAME}" COMMAND "test-${NAME}")
    set_tests_properties("test-${NAME}" PROPERTIES FIXTURES_REQUIRED "build-${NAME}")
endforeach()
