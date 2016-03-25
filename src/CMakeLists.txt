# TODO: Change this when 1.61 released
find_package(Boost 1.60 COMPONENTS system locale REQUIRED)
find_package(Threads REQUIRED)
find_package(YamlCpp REQUIRED)
include_directories("${PROJECT_SOURCE_DIR}/include")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
configure_file(config.h.in config.h)
add_library(cenisys SHARED
    command/defaultcommandhandlers.cpp
    server/cenisysserver.cpp
    server/cenisysserverlogger.cpp
    server/stdinreader.cpp
    server/stdoutlogger.cpp
    )
target_link_libraries(cenisys PUBLIC
    Threads::Threads
    Boost::boost
    Boost::system
    Boost::locale
    YamlCpp
    )
add_executable(cenisyslauncher main.cpp)
target_link_libraries(cenisyslauncher PUBLIC cenisys)
set_property(TARGET cenisyslauncher cenisys PROPERTY CXX_STANDARD 14)
set_property(TARGET cenisyslauncher cenisys PROPERTY CXX_STANDARD_REQUIRED YES)
install(TARGETS cenisys cenisyslauncher
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    )
