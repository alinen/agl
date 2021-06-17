file(GLOB SRC "*.h" "*.cpp" "mesh/*.h" "mesh/*.cpp") 
add_library(agl STATIC ${SRC})
