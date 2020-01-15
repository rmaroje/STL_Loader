#pragma once

#include <fstream>
#include <vector>
#include "Vertex.h"

//GLM
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Mesh;

using namespace std;

//This class represent simple example for loading binary .stl files.
class Loader
{
	public:
		Mesh* loadStl();
		Mesh* readStlBinary(ifstream& file);
		glm::vec3 setMeshCenter(Mesh* mesh);

	private:
		Mesh * coordinateFromVerts(uint32_t tri_count, vector<Vertex>& verts);
	
};

