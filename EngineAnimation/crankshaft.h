#ifndef CRANKSHAFT_H
#define CRANKSHAFT_H

#include "model.h"

namespace Models {
	namespace CrankshaftInternal {
		extern unsigned int vertexCount;
		extern std::vector <glm::vec3> vertices;
		extern std::vector <glm::vec2> uvs;
		extern std::vector <glm::vec3> normals;
	}

	class Crankshaft : public Model {
	public:
		Crankshaft();
		virtual ~Crankshaft();
		virtual void drawSolid();
	};

	extern Crankshaft crankshaft;
}

#endif#pragma once
