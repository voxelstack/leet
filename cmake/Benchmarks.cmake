find_package(Python COMPONENTS Interpreter)

add_custom_target(benchmarks)
add_custom_target(charts)
set(BENCHMARK_CHART "${CMAKE_SOURCE_DIR}/scripts/benchmark_chart.py")
set(BENCHMARK_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/results")
file(MAKE_DIRECTORY ${BENCHMARK_OUTPUT_DIR})

function(leet_benchmark SOURCE)
    string(REPLACE ".c" "" TARGET ${SOURCE})
    string(REPLACE "/" "." TARGET ${TARGET})
    string(PREPEND TARGET "bench.")

    set(RUNNER "${TARGET}.runner")

    add_executable(${TARGET} ${SOURCE})
    target_link_libraries(${TARGET} PRIVATE leet)
    target_compile_options(${TARGET} PRIVATE "-O0")
    # Benchmark macros execute functions that are defined at the end of the file.
    # Benchmark functions always return int and have no arguments.
    target_compile_options(${TARGET} PRIVATE "-Wno-implicit-function-declaration")

    set(BENCHMARK_BINARY "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.exe")
    set(BENCHMARK_CSV "${BENCHMARK_OUTPUT_DIR}/${TARGET}.csv")
    add_custom_command(
        OUTPUT ${BENCHMARK_CSV}
        DEPENDS ${TARGET} ${SOURCE}
        COMMAND
        ${BENCHMARK_BINARY} > ${BENCHMARK_CSV}
        WORKING_DIRECTORY ${BENCHMARK_OUTPUT_DIR}
    )
    add_custom_target(${RUNNER} DEPENDS ${BENCHMARK_CSV})
    add_dependencies(benchmarks ${RUNNER})
endfunction()

function(leet_chart)
    set(oneValueArgs SOURCE NAME)
    set(multiValueArgs RUNS)
    cmake_parse_arguments(
        CHART
        ""
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    string(REPLACE ".c" "" TARGET ${CHART_SOURCE})
    string(REPLACE "/" "." TARGET ${TARGET})
    set(CHART ${TARGET})
    string(PREPEND CHART "chart.")
    string(PREPEND TARGET "bench.")

    set(BENCHMARK_CSV "${BENCHMARK_OUTPUT_DIR}/${TARGET}.csv")
    set(CHART_JSON "${CMAKE_SOURCE_DIR}/docs/_charts/bench.${CHART_NAME}.json")
    add_custom_command(
        OUTPUT ${CHART_JSON}
        DEPENDS ${BENCHMARK_CSV}
        COMMAND
        ${Python_EXECUTABLE} ${BENCHMARK_CHART}
        ${BENCHMARK_CSV} ${CHART_JSON} ${CHART_RUNS}
    )
    add_custom_target(${CHART} DEPENDS ${CHART_JSON})
    add_dependencies(charts ${CHART})
endfunction()
