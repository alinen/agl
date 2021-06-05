set(SRC 
   AFramework.cpp  
   ARenderer.cpp  
   ASphere.cpp
   ACapsule.cpp
   ATriangleMesh.cpp
   ATorus.cpp
   ATeapot.cpp
   ACube.cpp
   APlane.cpp
   APyramid.cpp
   ACylinder.cpp
   ALinesMesh.cpp
   AGLSLProgram.cpp
   ACamera.cpp	  
   AImage.cpp	  
   ATexture.cpp	  
   ASkeletonDrawer.cpp	  
   )

add_library(g3d STATIC ${SRC})
