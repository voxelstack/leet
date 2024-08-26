
function(leet_test SOURCE)
    string(REPLACE ".c" "" NAME ${SOURCE})
    string(REPLACE "/" "." NAME ${NAME})
    string(PREPEND NAME "test.")

    set(BINARY "${NAME}.binary")
    set(BUILDER "${NAME}.builder")

    add_executable(${BINARY} ${SOURCE})
    target_link_libraries(${BINARY} PRIVATE leet)
    # Test macros execute functions that are defined at the end of the file.
    # Test functions always return int and have no arguments.
    target_compile_options(${BINARY} PRIVATE "-Wno-implicit-function-declaration")

    add_test(
        ${BUILDER}
        ${CMAKE_COMMAND}
        --build ${CMAKE_BINARY_DIR}
        --target ${BINARY}
    )
    set_tests_properties(${BUILDER} PROPERTIES FIXTURES_SETUP ${BUILDER})
    add_test(NAME ${NAME} COMMAND ${BINARY})
    set_tests_properties(${NAME} PROPERTIES FIXTURES_REQUIRED ${BUILDER})
endfunction()
