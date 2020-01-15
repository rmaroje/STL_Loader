#include "Loader.h"
#include "Mesh.h"
#include <iostream>


/*
====== STL file - structure ===============================================

begin
	char[80] (80 bytes) – Header
	unsigned long (4 bytes) – Number of triangles
foreach triangle
	float[3] (12 bytes) – Normal vector
	float[3] (12 bytes) – Vertex 1
	float[3] (12 bytes) – Vertex 2
	float[3] (12 bytes) – Vertex 3
	short (2 bytes) – Attribute byte count
end

Each triangle's data is 50 bytes long, but the last 2 bytes are rarely used.
*/


Mesh* Loader::loadStl()
{
	ifstream file("models/Predator.stl", ios::in | ios::binary);
	if (!file.is_open())
	{
		cout << "Can't open .stl file." << endl;
		return nullptr;
	}
	return readStlBinary(file);
}

Mesh* Loader::readStlBinary(ifstream& file)
{
	//Get file length
	file.seekg(0, ios::end);
	streamoff fileLength = file.tellg();
	file.seekg(0, ios::beg);
	//Skip header
	file.seekg(80);
	//Get number of triengles
	uint32_t triangleNumber;
	char buffer[4];
	file.read(buffer, 4);
	triangleNumber = *((int*)buffer);
	//Check file length
	if (fileLength != 84 + triangleNumber * 50)
	{
		cout << "File size not valid. " << "File size: " << fileLength << endl;
		return nullptr;
	}
	//Get vertices from file for each triangle
	vector<Vertex> verts;
	for (unsigned i = 0; i < triangleNumber; i++) {
		char buffer[50];
		if (file.end) {
			file.read(buffer, 50);
			//First skip normal vertices (facet + 12)
			Vertex vert1(*((float*)(buffer + 12)), *((float*)(buffer + 16)), *((float*)(buffer + 20)));
			Vertex vert2(*((float*)(buffer + 24)), *((float*)(buffer + 28)), *((float*)(buffer + 32)));
			Vertex vert3(*((float*)(buffer + 36)), *((float*)(buffer + 40)), *((float*)(buffer + 44)));
			verts.push_back(vert1);
			verts.push_back(vert2);
			verts.push_back(vert3);
		}
	}
	file.close();
	return coordinateFromVerts(triangleNumber, verts);
}

Mesh* Loader::coordinateFromVerts(uint32_t tri_count, vector<Vertex>& verts)
{
	std::vector<GLfloat> verticesCoordinate;
	for (auto v : verts)
	{
		verticesCoordinate.push_back(v.x);
		verticesCoordinate.push_back(v.y);
		verticesCoordinate.push_back(v.z);
	}
	return new Mesh(std::move(verticesCoordinate));
}

glm::vec3 Loader::setMeshCenter(Mesh* mesh)
{
	glm::vec3 lowerMesh(mesh->xmin(), mesh->ymin(), mesh->zmin());
	glm::vec3 upperMesh(mesh->xmax(), mesh->ymax(), mesh->zmax());
	glm::vec3 centerMesh = (lowerMesh + upperMesh) / 2.0f;
	return centerMesh;
}

