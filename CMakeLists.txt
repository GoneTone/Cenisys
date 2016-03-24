cmake_minimum_required(VERSION 3.1)
project(Cenisys VERSION 0.1)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
# Gettext directories
string(TOLOWER ${CMAKE_PROJECT_NAME} GETTEXT_PACKAGE)
set(PACKAGE_LOCALE_DIR "${CMAKE_INSTALL_PREFIX}/share/locale")

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(doc)
add_subdirectory(po)
