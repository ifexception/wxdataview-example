# CMakeList.txt : CMake project for wx-widgets-introduction, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project ("DataViewExample")

if (MSVC)
    include (${CMAKE_MODULE_PATH}/FindwxWidgetsVcpkg.cmake)
else (MSVC)
    find_package (wxWidgets REQUIRED COMPONENTS base core)
    include (${wxWidgets_USE_FILE})
endif ()

SET (SRC
    "main.cpp"
    )

# Add source to this project's executable.
add_executable (${PROJECT_NAME} WIN32 ${SRC})

# TODO: Add tests and install targets if needed.
target_compile_definitions (${PROJECT_NAME} PUBLIC
    _CRT_SECURE_NO_WARNINGS
    _UNICODE
    UNICODE
    WXUSINGDLL
    wxUSE_GUI=1
    __WXMSW__
    $<$<CONFIG:Debug>:_DEBUG>)

target_link_libraries (${PROJECT_NAME}
    ${wxWidgets_LIBRARIES})