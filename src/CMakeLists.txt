# TODO: Change this when 1.61 released
find_package(Boost 1.60 REQUIRED)
include_directories("${PROJECT_SOURCE_DIR}/include")
add_library(cenisys SHARED
    server/cenisysserver.cpp)
add_executable(cenisyslauncher main.cpp)
target_link_libraries(cenisyslauncher PUBLIC cenisys)
set_property(TARGET cenisyslauncher cenisys PROPERTY CXX_STANDARD 14)
set_property(TARGET cenisyslauncher cenisys PROPERTY CXX_STANDARD_REQUIRED YES)
install(TARGETS cenisys cenisyslauncher
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib)
