#ifndef VALVE_H
#define VALVE_H

#include "model.h"

namespace Models {
	namespace ValveInternal {
		extern unsigned int vertexCount;
		extern std::vector <glm::vec3> vertices;
		extern std::vector <glm::vec2> uvs;
		extern std::vector <glm::vec3> normals;
	}

	class Valve : public Model {
	public:
		Valve();
		virtual ~Valve();
		virtual void drawSolid();
	};

	extern Valve valve;
}

#endif
