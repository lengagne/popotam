file(GLOB all_valid_subdirs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*/CMakeLists.txt")

foreach(dir ${all_valid_subdirs})
    if(${dir} MATCHES "^([^/]*)/CMakeLists.txt")
        string(REGEX REPLACE "^([^/]*)/CMakeLists.txt" "\\1" dir_trimmed ${dir})
        message("cmake parsing : " ${dir_trimmed})
        add_subdirectory(${dir_trimmed})
     endif()
endforeach(dir)
