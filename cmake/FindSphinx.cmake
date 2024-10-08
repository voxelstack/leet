find_program(
    SPHINX_EXECUTABLE
    NAMES sphinx-build
    DOC "sphinx-build executable"
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    Sphinx
    "Failed to find sphinx-build executable"
    SPHINX_EXECUTABLE
)
