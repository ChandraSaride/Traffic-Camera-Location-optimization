cmake_minimum_required(VERSION 2.8)
project(ece650-a4)
if (EXISTS "${CMAKE_SOURCE_DIR}/minisat")
  add_subdirectory(minisat)
else()
  message (FATAL_ERROR "MiniSat not found. Use `git clone https://github.com/agurfinkel/minisat` to check it out.")
endif()
## ECE650 Specific configuration start here
## Project specific configurations go here
# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")
# expose minisat header files
include_directories(${CMAKE_SOURCE_DIR}/minisat)
file(COPY graph_app_disp.gp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY graph_time_disp.gp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY graph_time_ex.gp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/run)
# create the main executable
## add additional .cpp files if needed
add_executable(ece650-prj ece650-prj.cpp)
# link MiniSAT libraries
target_link_libraries(ece650-prj minisat-lib-static)
target_link_libraries(ece650-prj pthread)
#to copy gnuplot graph generation code to bin
install(PROGRAMS graph_time_disp.gp graph_time_ex.gp graph_app_disp.gp DESTINATION bin)
install(TARGETS ece650-prj RUNTIME DESTINATION bin)
