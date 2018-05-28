#include "piston.h"
#include <fstream>

namespace Models {

	Piston piston;

	Piston::Piston() {
		std::vector <glm::vec3> temp_vertices;
		std::vector <glm::vec2> temp_uvs; 
		std::vector <glm::vec3> temp_normals;
		std::vector <unsigned int> vertexIndices, uvIndices, normalIndices;
		
		vertices = PistonInternal::vertices;
		vertexCount = 0;
		
		int counter = 0;

		FILE * file;
		fopen_s(&file, "Objects/piston.obj", "r");
		if (file == NULL) {
			printf("Impossible to open the file !\n");
		}

		char lineHeader[128];
		while (1) {
			int res = fscanf_s(file, "%s", lineHeader, sizeof(lineHeader));
			if (res == EOF) {
				break;
			} else {
				if (strcmp(lineHeader, "v") == 0) {
					glm::vec3 vertex;
					fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
					temp_vertices.push_back(vertex);
				} 
				else if (strcmp(lineHeader, "vn") == 0) {
					glm::vec3 normal;
					fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
					temp_normals.push_back(normal);
				}
				else if (strcmp(lineHeader, "f") == 0) {
					counter++;
					unsigned int vertexIndex[3], normalIndex[3];
					int matches = fscanf_s(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
					if (matches != 6) {
						printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					}
					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);

					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[2]);
				}
			}	
		}

		float minx = 2000, miny = 2000, minz = 2000, maxx = -2000, maxy = -2000, maxz = -2000, srx, sry, srz;

		for (int i = 0; i < counter * 3; i++) {
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int normalIndex = normalIndices[i];
			glm::vec3 vertex = temp_vertices[vertexIndex - 1];
			glm::vec3 normal = temp_normals[normalIndex - 1];

			if (vertex.x > maxx) maxx = vertex.x;
			if (vertex.y > maxy) maxy = vertex.y;
			if (vertex.z > maxz) maxz = vertex.z;
			if (vertex.x < minx) minx = vertex.x;
			if (vertex.y < miny) miny = vertex.y;
			if (vertex.z < minz) minz = vertex.z;
		}
		srx = (maxx - minx) / 2.0f;
		sry = (maxy - miny) / 2.0f;
		srz = (maxz - minz) / 2.0f;


		for (int i = 0; i < counter*3; i++) {
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int normalIndex = normalIndices[i];
			glm::vec3 vertex = temp_vertices[vertexIndex - 1];
			glm::vec3 normal = temp_normals[normalIndex - 1];
			
			vertex.x = vertex.x - minx - srx;
			vertex.y = vertex.y - miny - sry;
			vertex.z = vertex.z - minz - srz;

			normal.x = -normal.x;
			normal.y = -normal.y;
			normal.z = -normal.z;
		
			vertices.push_back(vertex);
			normals.push_back(normal);
		}
	}

	Piston::~Piston() {
	}

	void Piston::drawSolid() {
		glEnable(GL_NORMALIZE);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
		glNormalPointer(GL_FLOAT, 0, &normals[0]);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	namespace PistonInternal {
		unsigned int vertexCount;

		std::vector <glm::vec3> vertices;
		std::vector <glm::vec2> uvs;
		std::vector <glm::vec3> normals;
	}
}