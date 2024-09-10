add_custom_target(programs)

function(leet_program TARGET SOURCE)
    add_executable(${TARGET} ${SOURCE})
    target_link_libraries(${TARGET} PRIVATE leet)

    add_dependencies(programs ${TARGET})
endfunction()
