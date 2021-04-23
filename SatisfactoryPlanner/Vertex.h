#pragma once
#include <glm/glm.hpp>

#include "Common.h"



enum class [[nodiscard]] EVertexAttributes
{
	POSITION,
	COLOR,
	UV,
	TEXTURE_INDEX,

	COUNT
};

struct [[nodiscard]] Vertex
{
	glm::vec4 position;
	glm::vec4 color;
	glm::vec2 uv;
	TextureIndexType textureIndex = -1;
};

