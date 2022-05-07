cmake_minimum_required(VERSION 3.21)
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
set(GLFW_VERSION 3.3.7)
set(BUILD_SHARED_LIBS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
FetchContent_Declare(glfw URL https://github.com/glfw/glfw/releases/download/${GLFW_VERSION}/glfw-${GLFW_VERSION}.zip)

###############################################################################
# glfwpp
set(GLFWPP_VERSION 207096552976035ae466382df6b5fc6905d06fbd)
FetchContent_Declare(glfwpp URL https://github.com/janekb04/glfwpp/archive/${GLFWPP_VERSION}.zip)
FetchContent_Populate(glfwpp)

###############################################################################
# imgui (docking branch)
set(IMGUI_VERSION d666a1d4737739274449dbe0e8558454bba82ec4)
FetchContent_Declare(imgui URL https://github.com/ocornut/imgui/archive/${IMGUI_VERSION}.zip)
FetchContent_Populate(imgui)

FetchContent_MakeAvailable(glfw)

add_library(${PROJECT_NAME} STATIC
    external/glad/src/glad.c
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${imgui_SOURCE_DIR}>
    $<BUILD_INTERFACE:${imgui_SOURCE_DIR}/backends>
    $<BUILD_INTERFACE:${glfwpp_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
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