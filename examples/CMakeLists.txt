file (GLOB_RECURSE EXAMPLES CONFIGURE_DEPENDS *.cpp)

foreach (example ${EXAMPLES})
  get_filename_component(example_name ${example} NAME_WLE)
  message(${example_name})
  add_executable(${example_name} ${example} ${SHADERS})
  target_link_libraries(${example_name} ${CORE})
  add_custom_command(                                                             
    TARGET ${example_name}                                                                    
    POST_BUILD                                                                    
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}                                         
    COMMAND python3 cpplint.py ${example}
    VERBATIM)
endforeach()
