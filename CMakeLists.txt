cmake_minimum_required(VERSION 3.16...3.21)
set(pname work6)
project(${pname})

option(Geant4_ui_vis "是否打开ui和vis的驱动" ON)
if(Geant4_ui_vis)
find_package(Geant4 REQUIRED ui_all vis_all)
else()
find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)


file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(result main.cc ${sources} ${headers})
target_link_libraries(result ${Geant4_LIBRARIES})

file(GLOB scripts  ${PROJECT_SOURCE_DIR}/*.mac ${PROJECT_SOURCE_DIR}/*.obj)
foreach(script ${scripts})
configure_file(${script} ${PROJECT_BINARY_DIR}/. COPYONLY)
endforeach()

add_custom_target(${pname} DEPENDS result)
install(TARGETS result DESTINATION bin)
