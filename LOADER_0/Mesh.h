#pragma once

//GLEW
#include <GL\glew.h>

//GLFW
#include <GLFW\glfw3.h>

#include <vector>

class Mesh
{
	public:
		Mesh(std::vector<GLfloat>&& vertices);

		float min(size_t start) const;
		float max(size_t start) const;

		float xmin() const { return min(0); }
		float ymin() const { return min(1); }
		float zmin() const { return min(2); }
		float xmax() const { return max(0); }
		float ymax() const { return max(1); }
		float zmax() const { return max(2); }

		bool empty() const;

		std::vector<GLfloat> getVertices() const
		{
			return this->vertices;
		}

	private:
		std::vector<GLfloat> vertices;

};

