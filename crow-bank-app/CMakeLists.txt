cmake_minimum_required(VERSION 3.5)
project(crow_bank_app)

set(CMAKE_CXX_STANDARD 17)

add_executable(crow_bank_app main.cpp)

target_include_directories(crow_bank_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Platform-specific configurations
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # For Windows cross-compilation: Static linking to avoid DLL dependencies
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    target_link_libraries(crow_bank_app ws2_32 mswsock)
    
    # Copy public folder to build directory for Windows
    add_custom_command(TARGET crow_bank_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/public
        ${CMAKE_BINARY_DIR}/public
        COMMENT "Copying public folder to Windows build directory")
else()
    # For Linux (ELF)
    target_link_libraries(crow_bank_app pthread)
    
    # Copy public folder to build directory for Linux
    add_custom_command(TARGET crow_bank_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/public
        ${CMAKE_BINARY_DIR}/public
        COMMENT "Copying public folder to Linux build directory")
endif()
