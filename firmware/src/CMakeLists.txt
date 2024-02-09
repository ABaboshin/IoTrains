cmake_minimum_required(VERSION 3.13)

project(dcc C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

function(embed_resource resource_file_name source_file_name variable_name)

        file(READ ${resource_file_name} hex_content HEX)

        string(REPEAT "[0-9a-f]" 32 column_pattern)
        string(REGEX REPLACE "(${column_pattern})" "\\1\n" content "${hex_content}")

        string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1, " content "${content}")

        string(REGEX REPLACE ", $" "" content "${content}")

        set(array_definition "static const unsigned char ${variable_name}[] PROGMEM =\n{\n${content}\n};")

        set(source "// Auto generated file.\n${array_definition}\n")

        file(WRITE "${source_file_name}" "${source}")

endfunction()

embed_resource(test.mp3 mp3.h outputmp3)

find_package(Protobuf REQUIRED)
message(STATUS "Using Protocol Buffers ${protobuf_VERSION}")
message(STATUS "Using Protocol Buffers ${PROTO_SRCS}")
message(STATUS "Using Protocol Buffers ${PROTO_HDRS}")
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ../protobuf/controlunit.proto)
