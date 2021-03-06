#include "Mesh.h"


Mesh::Mesh(std::vector<GLfloat>&& v)
	: vertices(std::move(v))
{

}

float Mesh::min(size_t start) const
{
	if (start >= vertices.size())
	{
		return -1;
	}
	float v = vertices[start];
	for (size_t i = start; i < vertices.size(); i += 3)
	{
		v = fmin(v, vertices[i]);
	}
	return v;
}

float Mesh::max(size_t start) const
{
	if (start >= vertices.size())
	{
		return 1;
	}
	float v = vertices[start];
	for (size_t i = start; i < vertices.size(); i += 3)
	{
		v = fmax(v, vertices[i]);
	}
	return v;
}

bool Mesh::empty() const
{
	return vertices.size() == 0;
}

