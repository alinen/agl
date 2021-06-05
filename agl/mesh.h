
#ifndef meshmodel_H_
#define meshmodel_H_

#include "AGLM.h"
#include "triangleMesh.h"

namespace agl {
   class Mesh : public TriangleMesh
   {
   public:

      Mesh(const std::string& filename);
      Mesh();

      virtual ~Mesh();

      // Initialize this object with the given file
      // Returns true if successfull. false otherwise.
      bool loadPLY(const std::string& filename);

      // Return the minimum point of the axis-aligned bounding box
      glm::vec3 getMinBounds() const;

      // Return the maximum point of the axis-aligned bounding box
      glm::vec3 getMaxBounds() const;

      // Called during load.  Because we want the model
      // to always fit in a 10x10x10 box, we need to translate and scale
      // our vertices so that we can calculate our normals correctly.
      glm::mat4 translateAndScale() const;

   protected:
      void clear(); // clear bounding box

   protected:

      float myMinx, myMiny, myMinz;
      float myMaxx, myMaxy, myMaxz;
   };
}

#endif
