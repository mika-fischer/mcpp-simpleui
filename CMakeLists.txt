cmake_minimum_required(VERSION 3.27)
project(mcpp-simpleui)
set(PROJECT_ALIAS mcpp::simpleui)

if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(is_toplevel TRUE)
else ()
    set(is_toplevel FALSE)
endif ()

option(${PROJECT_NAME}_WITH_TESTS   "Build tests"     ${is_toplevel})
option(${PROJECT_NAME}_WITH_INSTALL "Install project" ${is_toplevel})

###############################################################################
# Third party dependencies
include(FetchContent)

###############################################################################
# glfw
set(GLFW_VERSION 3.3.8)
set(BUILD_SHARED_LIBS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
FetchContent_Declare(glfw URL https://github.com/glfw/glfw/releases/download/${GLFW_VERSION}/glfw-${GLFW_VERSION}.zip)
FetchContent_MakeAvailable(glfw)

###############################################################################
# glfwpp
set(GLFWPP_VERSION 17b0ca06799fec64f63f5006fe45209ce33aace0)
FetchContent_Declare(glfwpp URL https://github.com/janekb04/glfwpp/archive/${GLFWPP_VERSION}.zip)
FetchContent_Populate(glfwpp)

###############################################################################
# imgui (docking branch)
set(IMGUI_VERSION 2a6d7b1eaa1851e5b53d8c7a361a8d54f5a9ae8c)
FetchContent_Declare(imgui URL https://github.com/ocornut/imgui/archive/${IMGUI_VERSION}.zip)
FetchContent_Populate(imgui)

include(external/cmake-bin2h/bin2h.cmake)
set(font_dir ${CMAKE_CURRENT_BINARY_DIR}/fonts)
bin2h(SOURCE_FILE   ${imgui_SOURCE_DIR}/misc/fonts/DroidSans.ttf
      HEADER_FILE   ${font_dir}/droidsans.h
      VARIABLE_NAME "FONT_DROIDSANS")

add_library(${PROJECT_NAME} STATIC
    external/glad/src/glad.c
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    src/context.cpp
    src/fonts.cpp
    src/frame_scope.cpp
    src/image.cpp
    src/main_window.cpp
)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
    $<BUILD_INTERFACE:${imgui_SOURCE_DIR}>
    $<BUILD_INTERFACE:${imgui_SOURCE_DIR}/backends>
    $<BUILD_INTERFACE:${glfwpp_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include>
)
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE ${font_dir}
)
target_compile_definitions(${PROJECT_NAME} PUBLIC GLFW_INCLUDE_NONE)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES EXPORT_NAME ${PROJECT_ALIAS})
add_library(${PROJECT_ALIAS} ALIAS ${PROJECT_NAME})

# TODO
# if (${PROJECT_NAME}_WITH_INSTALL)
#     install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config)
#     install(EXPORT ${PROJECT_NAME}-config DESTINATION share/cmake/${PROJECT_NAME})
#     install(DIRECTORY include DESTINATION ".")
# endif ()

if (${PROJECT_NAME}_WITH_TESTS)
    include(CTest)
    add_subdirectory(tests)
endif ()