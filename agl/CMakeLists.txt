#file(GLOB SRC "*.h" "*.cpp") 
set(SRC 
  window.cpp
  renderer.cpp
  sphere.cpp
  skybox.cpp
  image.cpp
  mesh.cpp
  aglm.cpp
  triangle_mesh.cpp
  )
add_library(agl STATIC ${SRC})
