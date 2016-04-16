option(BUILD_TEST "Build and install tests" OFF)
if(BUILD_TEST)
    find_package(Boost 1.60
        COMPONENTS iostreams
        unit_test_framework
        REQUIRED
        )
    include_directories("${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/src")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    add_executable(test_runner
        main.cpp
        nbt.cpp
        )
    target_link_libraries(test_runner
        cenisys
        Boost::boost
        Boost::iostreams
        Boost::unit_test_framework
        )
    if(NOT Boost_USE_STATIC_LIBS)
        target_compile_definitions(test_runner PRIVATE BOOST_TEST_DYN_LINK)
    endif()
    set_property(TARGET test_runner PROPERTY CXX_STANDARD 14)
    set_property(TARGET test_runner PROPERTY CXX_STANDARD_REQUIRED YES)
    install(TARGETS test_runner
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        )
endif()
