file(GLOB SRC "*.h" "*.cpp" "mesh/*.h" "mesh/*.cpp") 
add_library(agl STATIC ${SRC})

add_custom_command(
  TARGET agl
  POST_BUILD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMAND doxygen Doxyfile
  COMMAND python3 cpplint.py --quiet ${SRC}
  VERBATIM)
