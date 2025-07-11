cmake_minimum_required(VERSION 3.5)
project(crow_bank_app)

set(CMAKE_CXX_STANDARD 17)

add_executable(crow_bank_app main.cpp)

target_include_directories(crow_bank_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Platform-specific configurations
if(WIN32)
    # For Windows: Static linking to avoid DLL dependencies
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    target_link_libraries(crow_bank_app ws2_32 mswsock)
else()
    # For Linux (ELF)
    target_link_libraries(crow_bank_app pthread)
endif()
