#ifndef PISTON_H
#define PISTON_H

#include "model.h"

namespace Models {
	namespace PistonInternal {
		extern unsigned int vertexCount;
		extern std::vector <glm::vec3> vertices;
		extern std::vector <glm::vec2> uvs;
		extern std::vector <glm::vec3> normals;
	}

	class Piston : public Model {
	public:
		Piston();
		virtual ~Piston();
		virtual void drawSolid();
	};

	extern Piston piston;
}

#endif#pragma once
