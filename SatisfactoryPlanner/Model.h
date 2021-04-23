#pragma once
#include <vector>

#include "Vertex.h"



struct [[nodiscard]] Model
{
	std::vector<Vertex> vertices;
	std::vector<VertexIndexType> indices;
};
