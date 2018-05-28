#ifndef CONROD_H
#define CONROD_H

#include "model.h"

namespace Models {
	namespace ConrodInternal {
		extern unsigned int vertexCount;
		extern std::vector <glm::vec3> vertices;
		extern std::vector <glm::vec2> uvs;
		extern std::vector <glm::vec3> normals;
	}

	class Conrod : public Model {
	public:
		Conrod();
		virtual ~Conrod();
		virtual void drawSolid();
	};

	extern Conrod conrod;
}

#endif#pragma once
