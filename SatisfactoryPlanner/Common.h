#pragma once
#include <type_traits>


using VertexIndexType = unsigned int;
using TextureIndexType = float;
using ObjectHandle = size_t;

using InputCallbackHandle = size_t;


template<typename T>
[[nodiscard]] constexpr unsigned int get_opengl_type() noexcept
{
	if constexpr (std::is_same_v<T, char>)
	{
		return 0x1400;
		//return GL_BYTE;
	}
	if constexpr (std::is_same_v<T, unsigned char>)
	{
		return 0x1401;
		//return GL_UNSIGNED_BYTE;
	}
	if constexpr (std::is_same_v<T, short>)
	{
		return 0x1402;
		//return GL_SHORT;
	}
	if constexpr (std::is_same_v<T, unsigned short>)
	{
		return 0x1403;
		//return GL_UNSIGNED_SHORT;
	}
	if constexpr (std::is_same_v<T, int>)
	{
		return 0x1404;
		//return GL_INT;
	}
	if constexpr (std::is_same_v<T, unsigned int>)
	{
		return 0x1405;
		//return GL_UNSIGNED_INT;
	}
	if constexpr (std::is_same_v<T, float>)
	{
		return 0x1406;
		//return GL_FLOAT;
	}
	if constexpr (std::is_same_v<T, double>)
	{
		return 0x140A;
		//return GL_DOUBLE;
	}

	return 0;
}
