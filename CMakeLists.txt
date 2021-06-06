project(CS312-Particles)
cmake_minimum_required(VERSION 2.8.11)

if (WIN32) # Include win64 platforms

  find_package(OpenGL REQUIRED)
  find_library(GLEW NAMES glew32s PATHS external/lib/x64)
  find_library(GLFW NAMES glfw3 PATHS external/lib)

  set(INCLUDE_DIRS
     ../../external/include )

  set(LIBRARY_DIRS
    lib
    ../../external/lib)

  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_FLAGS 
     "/wd4018 /wd4244 /wd4305 
     /D_CRT_SECURE_NO_WARNINGS 
     /D_CRT_NONSTDC_NO_DEPRECATE 
     /D NOMINMAX /DGLEW_STATIC
     /EHsc")

  set(VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
  set(CMAKE_EXE_LINKER_FLAGS "/NODEFAULTLIB:\"MSVCRT\" /NODEFAULTLIB:\"LIBCMT\"")

  set(CORE ${GLEW} ${GLFW} opengl32.lib)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
  set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

elseif (APPLE)

  set(CMAKE_MACOSX_RPATH 1)
  set(CMAKE_CXX_FLAGS "-Wall -Wno-deprecated-declarations -Wno-reorder-ctor -Wno-unused-variable -g -stdlib=libc++ -std=c++14")
  set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
  find_library(GL_LIB OpenGL)
  find_library(GLFW glfw)
  add_definitions(-DAPPLE)

  set(INCLUDE_DIRS
    /System/Library/Frameworks
    .
    external/include
    /usr/local/include)

  set(CORE agl ${GLFW} ${GL_LIB})

elseif (UNIX)

  set(CMAKE_CXX_FLAGS "-Wall -g -std=c++11 -Wno-comment")
  set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

  FIND_PACKAGE(OpenGL REQUIRED) 
  FIND_PACKAGE(GLEW REQUIRED)

  set(INCLUDE_DIRS
    external/include)

  set(LIBRARY_DIRS
    external/lib
    /usr/X11R6/lib
    /usr/local/lib
    lib)

  add_definitions(-DUNIX)
  set(CORE GLEW glfw glut GL GLU X11)

endif()

include_directories(${INCLUDE_DIRS})
link_directories(${LIBRARY_DIRS})
add_subdirectory(agl)

set(SHADERS
    shaders/cubemap.vs
    shaders/cubemap.fs
    shaders/phong.vs
    shaders/phong.fs
    shaders/billboard.fs
    shaders/billboard.vs)

add_executable(sphere examples/hello-agl/sphere.cpp ${SHADERS})
target_link_libraries(sphere ${CORE})

add_executable(sprites examples/hello-agl/sprites.cpp ${SHADERS})
target_link_libraries(sprites ${CORE})

add_executable(cubemap examples/hello-agl/cubemap.cpp ${SHADERS})
target_link_libraries(cubemap ${CORE})
