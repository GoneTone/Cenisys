# TODO: Change this when 1.61 released
find_package(Boost 1.60 COMPONENTS system REQUIRED)
find_package(Threads REQUIRED)
include_directories("${PROJECT_SOURCE_DIR}/include")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_library(cenisys SHARED
    server/cenisysserver.cpp
    server/cenisysserverlogger.cpp
    server/stdinreader.cpp
    server/stdoutlogger.cpp)
target_include_directories(cenisys PUBLIC
    ${Boost_INCLUDE_DIRS})
target_link_libraries(cenisys PUBLIC
    Threads::Threads
    ${Boost_SYSTEM_LIBRARY})
add_executable(cenisyslauncher main.cpp)
target_link_libraries(cenisyslauncher PUBLIC cenisys)
set_property(TARGET cenisyslauncher cenisys PROPERTY CXX_STANDARD 14)
set_property(TARGET cenisyslauncher cenisys PROPERTY CXX_STANDARD_REQUIRED YES)
install(TARGETS cenisys cenisyslauncher
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib)
