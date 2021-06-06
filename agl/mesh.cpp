// Copyright 2020, Savvy Sine, alinen

#include "agl/mesh.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace agl {

Mesh::Mesh(const std::string& filename) :
  myMinx(100000), myMiny(100000), myMinz(100000),
  myMaxx(0), myMaxy(0), myMaxz(0) {
  loadPLY(filename);
}

Mesh::Mesh() :
  myMinx(100000), myMiny(100000), myMinz(100000),
  myMaxx(0), myMaxy(0), myMaxz(0) {
}


Mesh::~Mesh() {
}

void Mesh::clear() {
  myMinx = myMiny = myMinz = 100000;
  myMaxx = myMaxy = myMaxz = 0;
}

bool Mesh::loadPLY(const std::string& filename) {
  std::ifstream fp(filename);

  if (!fp) {
    std::cout << "Cannot load file: " + filename << std::endl;
    return false;
  }

  clear();

  int numVertices;
  std::vector<GLfloat> vertices;
  std::vector<GLfloat> vertexNormals;
  std::vector<GLfloat> uvs;

  int numFaces;
  std::vector<GLuint> faces;

  std::string buff;
  getline(fp, buff);
  if (buff != "ply") {
    std::cout << filename << " is not a Ply file.\n";
    return false;
  }

  int vertexIndex = 0;
  int normalIndex = 0;
  int uvIndex = 0;
  int faceIndex = 0;
  bool readingData = false;

  while (getline(fp, buff)) {
    std::stringstream ss(buff);
    if (!readingData) {
      std::string firstToken;
      ss >> firstToken;
      if (firstToken == "element") {
        std::string type;
        int amount = -1;
        ss >> type;
        ss >> amount;
        if (type == "vertex") {
          numVertices = amount;
        } else if (type == "face") {
          numFaces = amount;
        }
      } else if (firstToken == "end_header") {
        readingData = true;
      }
    } else {
      if (vertices.size() < numVertices*3) {
        float x, y, z, nx, ny, nz;
        ss >> x;
        ss >> y;
        ss >> z;
        ss >> nx;
        ss >> ny;
        ss >> nz;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);

        vertexNormals.push_back(nx);
        vertexNormals.push_back(ny);
        vertexNormals.push_back(nz);

        if (x < myMinx) myMinx = x;
        if (y < myMiny) myMiny = y;
        if (z < myMinz) myMinz = z;

        if (x > myMaxx) myMaxx = x;
        if (y > myMaxy) myMaxy = y;
        if (z > myMaxz) myMaxz = z;
      } else if (faces.size() < numFaces*3) {
        int num = 0;
        int i0, i1, i2;
        ss >> num;
        if (num != 3) {
          std::cout << "Only triangles are supported\n";
          return false;
        }
        ss >> i0;
        ss >> i1;
        ss >> i2;

        faces.push_back(i0);
        faces.push_back(i1);
        faces.push_back(i2);
      }
    }
  }

  initBuffers(&faces, &vertices, &vertexNormals, 0, 0);
  return true;
}

glm::vec3 Mesh::getMinBounds() const {
  return glm::vec3(myMinx, myMiny, myMinz);
}

glm::vec3 Mesh::getMaxBounds() const {
  return glm::vec3(myMaxx, myMaxy, myMaxz);
}

glm::mat4 Mesh::translateAndScale() const {
  float distancex = fabs(myMaxx-myMinx);
  float distancey = fabs(myMaxy-myMiny);
  float distancez = fabs(myMaxz-myMinz);

  // Calculate the largest dimension in the model.  We will
  // use this to scale the model so it fits into a 10x10x10
  // box.  Specifically, we will use 10/scale_factor to scale
  // the model to the right proportions.
  float scale_factor = distancex > distancey? distancex : distancey;
  scale_factor = scale_factor > distancez? scale_factor : distancez;

  // x,y,z represent the center of the model
  float x = myMinx + distancex/2.0;
  float y = myMiny + distancey/2.0;
  float z = myMinz + distancez/2.0;

  float scaleFactor = 10 / scale_factor;
  glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(scaleFactor));
  glm::mat4 translate = glm::translate(glm::mat4(1), glm::vec3(-x, -y, -z));

  return scale * translate;
}

}  // namespace agl
