
#ifndef Material_H_
#define Material_H_

namespace agl 
{

struct Material
{
public:
   virtual void apply(Shader s);
};

struct Phong : public Material
{
public:
   void apply(Shader s) override;

   GLuint texture1;
   GLuint texture2;
   GLuint cubemap;
   glm::vec4 light;
   glm::vec3 diffuse; 
   glm::vec3 specular; 
   glm::vec3 ambient;
   float shininess;
};

/*
struct Sprite : public Material
{
public:
   void apply(Shader s) override;
   GLuint texture;
   glm::vec4 color; // 4th component is alpha
};
*/

} // end namespace g3d

#endif


