#ifndef Renderer_H_
#define Renderer_H_

#include "AGL.h"
#include "AGLM.h"
#include "image.h"
#include "skybox.h"
#include "mesh.h"
#include <string>

namespace agl {

   enum BlendMode {DEFAULT, ADD, ALPHA};

   class Renderer
   {
   public:
      Renderer();
      virtual void init();
      virtual bool initialized() const;
      
      virtual ~Renderer();

      virtual GLuint loadTexture(const std::string& imageName);
      virtual GLuint loadCubemap(const std::vector<std::string>& imageNames);
      virtual GLuint loadShader(const std::string& vertex, const std::string& fragment);

      virtual void perspective(float fovRadians, float aspect, float near, float far); 
      virtual void ortho(float minx, float maxx, float miny, float maxy, float minz, float maxz); 
      virtual void lookAt(const glm::vec3& lookfrom, const glm::vec3& lookat);

      glm::vec3 cameraPosition() const;

/*
      virtual void begin(const std::string& shaderName); 
      virtual void end(); 

      virtual void setUniform(const std::string& name, int value); 
      virtual void setUniform(const std::string& name, fliat value); 
      virtual void setUniform(const std::string& name, const glm::vec3& value); 
      virtual void setUniform(const std::string& name, const glm::vec4& value); 
      virtual void setUniform(const std::string& name, const glm::mat3& value); 
      virtual void setUniform(const std::string& name, const glm::mat4& value); 
*/
      //virtual void texture(GLuint textureId); // NEW
      //virtual void color(const vec4& color); // NEW

      // draw many sprites 
      virtual void begin(GLuint textureId, BlendMode mode);
      virtual void quad(const glm::vec3& pos, const glm::vec4& color, float size);
      virtual void end(); // reset draw state 

      // draw a mesh
      virtual void mesh(const glm::mat4& trs, const agl::Mesh& m);
      
      // draw the cubemap
      virtual void skybox();


   protected:

      virtual void initBillboards();
      virtual void initCubemap();
      virtual void initMesh();

      virtual void blendMode(BlendMode mode);
      std::string loadShaderFromFile(const std::string& fileName);

   protected:
      GLuint mBBShaderId;
      GLuint mBBVboPosId; // quad rendering
      GLuint mBBVaoId;  // quad renderering

      GLuint mCMShaderId; // skybox rendering
      GLuint mCubemap; // skybox rendering
      SkyBox* mSkybox; // skybox rendering

      GLuint mMShaderId; // main rendering style (phong-based shader)
      
      glm::mat4 mProjectionMatrix;
      glm::mat4 mViewMatrix;
      glm::vec3 mLookfrom;
      bool mInitialized;
   };
}

#endif
