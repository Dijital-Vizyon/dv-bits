cmake_minimum_required(VERSION 3.15)
project(cpp_vision VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(Threads REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/generated
    ${Protobuf_INCLUDE_DIRS}
    ${_GRPC_CPP_PLUGIN_INCLUDE_DIRS}
)

# Generated protobuf files
set(PROTO_FILES
    ../proto/ai_transport.proto
)

# Generate protobuf and gRPC files
set(PROTO_SRCS)
set(PROTO_HDRS)
set(GRPC_SRCS)
set(GRPC_HDRS)

foreach(proto_file ${PROTO_FILES})
    get_filename_component(proto_name ${proto_file} NAME_WE)
    get_filename_component(proto_path ${proto_file} PATH)
    
    set(proto_src "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/${proto_name}.pb.cc")
    set(proto_hdr "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/${proto_name}.pb.h")
    set(grpc_src "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/${proto_name}.grpc.pb.cc")
    set(grpc_hdr "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/${proto_name}.grpc.pb.h")
    
    list(APPEND PROTO_SRCS ${proto_src})
    list(APPEND PROTO_HDRS ${proto_hdr})
    list(APPEND GRPC_SRCS ${grpc_src})
    list(APPEND GRPC_HDRS ${grpc_hdr})
    
    add_custom_command(
        OUTPUT ${proto_src} ${proto_hdr} ${grpc_src} ${grpc_hdr}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/generated
        COMMAND protobuf::protoc
            --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/../proto
            --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}/src/generated
            --grpc_out=${CMAKE_CURRENT_SOURCE_DIR}/src/generated
            --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
            ${proto_file}
        DEPENDS ${proto_file}
        COMMENT "Generating C++ and gRPC code from ${proto_file}"
    )
endforeach()

# Source files
set(SOURCES
    src/main.cpp
    src/vision_service.cpp
    src/latent_quantizer.cpp
    ${PROTO_SRCS}
    ${GRPC_SRCS}
)

# Executable
add_executable(cpp_vision ${SOURCES})

# Link libraries
target_link_libraries(cpp_vision
    PRIVATE
    ${Protobuf_LIBRARIES}
    gRPC::grpc++
    gRPC::grpc++_reflection
    Threads::Threads
)

# Compiler flags
target_compile_options(cpp_vision PRIVATE
    -Wall -Wextra -O3 -march=native
)
