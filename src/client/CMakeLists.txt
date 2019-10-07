cmake_minimum_required(VERSION 3.13)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../../bin/)
set (CMAKE_CXX_STANDARD 11)

find_package(Protobuf REQUIRED)
find_package(Threads)

set (SOURCES
    multicast_client_single_thread.cpp
    execution_manager.cpp
    MulticastClient.cpp
    RefereeClient.cpp
    referee_client_single_thread.cpp
    VisionClient.cpp
    viewer_server.cpp
    SimClient.cpp
    Kinematic.cpp
    Master.cpp
    joystick/Joystick.cpp
)

add_library(ssl_client ${SOURCES})
target_link_libraries (ssl_client PUBLIC ${CONAN_LIBS} Threads::Threads)
target_include_directories(ssl_client PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)

add_executable (referee referee.cpp ${SOURCES})
target_link_libraries (referee ssl_client)

add_executable (referee_st referee_st.cpp ${SOURCES})
target_link_libraries (referee_st ssl_client)

add_executable (vision vision.cpp ${SOURCES})
target_link_libraries (vision ssl_client)

add_executable (vision_st vision_st.cpp ${SOURCES})
target_link_libraries (vision_st ssl_client)

add_executable (sim sim.cpp ${SOURCES})
target_link_libraries (sim ssl_client)

add_executable (kinematic kinematic.cpp ${SOURCES})
target_link_libraries (kinematic ssl_client )

add_executable (viewer_threaded viewer_threaded.cpp ${SOURCES})
target_link_libraries (viewer_threaded ssl_client )

add_executable (master master.cpp ${SOURCES})
target_link_libraries (master ssl_client)

if(BUILD_TESTS)
  set (TEST_SOURCES
    tests/test_execution_manager.cpp
  )
  foreach(test_source ${TEST_SOURCES})
    get_filename_component(name_test ${test_source} NAME_WE)
    add_executable(${name_test} ${test_source})
    add_test(NAME ${name_test} COMMAND ${name_test})
    set_target_properties(${name_test} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests/")
    target_link_libraries(${name_test} ssl_client)
  endforeach(test_source)
endif()
