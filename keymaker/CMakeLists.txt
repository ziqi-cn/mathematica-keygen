# CMakeList.txt: keymaker 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("keymaker")

MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "Platform option: Windows")
    option (WIN32_ENV "option for win32" ON)
else ()
    MESSAGE(STATUS "Platform option: Linux ")
    option (LINUX_ENV "option for linux" ON)
endif ()

configure_file ("${PROJECT_SOURCE_DIR}/config.h.in" "${PROJECT_SOURCE_DIR}/config.h")

# 将源代码添加到此项目的可执行文件。
add_executable (keymaker "keymaker.c" "keygen.h")
add_executable (gethash "gethash.c" "keygen.h")

# TODO: 如有需要，请添加测试并安装目标。
