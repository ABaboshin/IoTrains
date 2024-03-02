cmake_minimum_required(VERSION 3.13)

function(embed_resource resource_file_name source_file_name variable_name)

        file(READ ${resource_file_name} hex_content HEX)

        string(REPEAT "[0-9a-f]" 32 column_pattern)
        string(REGEX REPLACE "(${column_pattern})" "\\1\n" content "${hex_content}")

        string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1, " content "${content}")

        string(REGEX REPLACE ", $" "" content "${content}")

        set(array_definition "static const /*std::vector<unsigned char>*/ unsigned char ${variable_name}[] PROGMEM =\n{\n${content}\n};")

        set(source "// Auto generated file.\n${array_definition}\n")

        file(WRITE "${source_file_name}" "${source}")

endfunction()

embed_resource(test.mp3 mp3.h outputmp3)
