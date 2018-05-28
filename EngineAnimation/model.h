
#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "constants.h"

namespace Models {

	class Model {
	public:
		int vertexCount;
		std::vector <glm::vec3> vertices;
		std::vector <glm::vec3> normals;
		/*float *vertexNormals;
		float *texCoords;
		float *colors;*/


		virtual void drawSolid() = 0;
		virtual void drawWire();
	};
}

#endif