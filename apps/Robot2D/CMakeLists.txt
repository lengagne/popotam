include_directories( include)

add_executable (Robot2D 
                src/test_Robot_2D.cpp
                src/Problem1.cpp
                src/Problem4.cpp
                src/Problem7.cpp
                src/Problem1Torque.cpp
                src/Problem4Torque.cpp
                src/Problem7Torque.cpp
                src/contruct_filename_2D.cpp)

target_link_libraries(	Robot2D
                        ${POPOTAM_LIBRARIES}
                        )
