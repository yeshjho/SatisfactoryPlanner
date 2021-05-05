#include "BatchRenderer.h"

#include <iostream>
#include <numeric>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Model.h"



BatchRenderer::BatchRenderer(const std::filesystem::path& textureSetFilePath)
	: mTextureSet(textureSetFilePath)
{
	mVertices.reserve(INITIAL_VERTEX_COUNT);
	mIndices.reserve(INITIAL_INDEX_COUNT);


	glCreateBuffers(1, &mVboHandle);
	glNamedBufferStorage(mVboHandle, sizeof(decltype(mVertices)::value_type) * mVertices.capacity(), nullptr, GL_DYNAMIC_STORAGE_BIT);


	glCreateVertexArrays(1, &mVaoHandle);
	using VertexType = Vertex;

	using VertexPositionType = decltype(VertexType::position);
	constexpr size_t vertexPositionOffset = offsetof(VertexType, position);
	const int positionAttributeIndex = static_cast<int>(EVertexAttributes::POSITION);

	using VertexColorType = decltype(VertexType::color);
	constexpr size_t vertexColorOffset = offsetof(VertexType, color);
	const int colorAttributeIndex = static_cast<int>(EVertexAttributes::COLOR);

	using VertexUVType = decltype(VertexType::uv);
	constexpr size_t vertexUVOffset = offsetof(VertexType, uv);
	const int uvAttributeIndex = static_cast<int>(EVertexAttributes::UV);

	using VertexTextureIndexType = decltype(VertexType::textureIndex);
	constexpr size_t vertexTextureIndexOffset = offsetof(VertexType, textureIndex);
	const int textureIndexAttributeIndex = static_cast<int>(EVertexAttributes::TEXTURE_INDEX);

	glEnableVertexArrayAttrib(mVaoHandle, positionAttributeIndex);
	glVertexArrayVertexBuffer(mVaoHandle, positionAttributeIndex, mVboHandle, vertexPositionOffset, sizeof(VertexType));
	glVertexArrayAttribFormat(mVaoHandle, positionAttributeIndex, VertexPositionType::length(), get_opengl_type<VertexPositionType::value_type>(), GL_FALSE, 0);
	glVertexArrayAttribBinding(mVaoHandle, positionAttributeIndex, positionAttributeIndex);

	glEnableVertexArrayAttrib(mVaoHandle, colorAttributeIndex);
	glVertexArrayVertexBuffer(mVaoHandle, colorAttributeIndex, mVboHandle, vertexColorOffset, sizeof(VertexType));
	glVertexArrayAttribFormat(mVaoHandle, colorAttributeIndex, VertexColorType::length(), get_opengl_type<VertexColorType::value_type>(), GL_FALSE, 0);
	glVertexArrayAttribBinding(mVaoHandle, colorAttributeIndex, colorAttributeIndex);

	glEnableVertexArrayAttrib(mVaoHandle, uvAttributeIndex);
	glVertexArrayVertexBuffer(mVaoHandle, uvAttributeIndex, mVboHandle, vertexUVOffset, sizeof(VertexType));
	glVertexArrayAttribFormat(mVaoHandle, uvAttributeIndex, VertexUVType::length(), get_opengl_type<VertexUVType::value_type>(), GL_FALSE, 0);
	glVertexArrayAttribBinding(mVaoHandle, uvAttributeIndex, uvAttributeIndex);

	glEnableVertexArrayAttrib(mVaoHandle, textureIndexAttributeIndex);
	glVertexArrayVertexBuffer(mVaoHandle, textureIndexAttributeIndex, mVboHandle, vertexTextureIndexOffset, sizeof(VertexType));
	glVertexArrayAttribFormat(mVaoHandle, textureIndexAttributeIndex, 1, get_opengl_type<VertexTextureIndexType>(), GL_FALSE, 0);
	glVertexArrayAttribBinding(mVaoHandle, textureIndexAttributeIndex, textureIndexAttributeIndex);


	glCreateBuffers(1, &mEboHandle);
	glNamedBufferStorage(mEboHandle, sizeof(decltype(mIndices)::value_type) * mIndices.capacity(), nullptr, GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(mVaoHandle, mEboHandle);
}


BatchRenderer::~BatchRenderer()
{
	glDeleteBuffers(1, &mVboHandle);
	glDeleteVertexArrays(1, &mVaoHandle);
	glDeleteBuffers(1, &mEboHandle);
}


ObjectHandle BatchRenderer::AddObject(const Model& model, const glm::mat4& model2WorldTransform)
{
	return addObject(model, model2WorldTransform);
}


ObjectHandle BatchRenderer::addObject(const Model& model, const glm::mat4& model2WorldTransform, const ObjectHandle overwriteHandle)
{
	const ObjectHandle newHandle = overwriteHandle == std::numeric_limits<ObjectHandle>::max() ? mNextObjectHandle++ : overwriteHandle;
	
	// TODO: transform vs. insert for_each

	const VertexIndexType indexStartPos = static_cast<VertexIndexType>(mVertices.size());
	if (!mIndices.empty())
	{
		mIndices.push_back(mIndices.back());
		mIndices.push_back(indexStartPos + model.indices.front());
	}

	mVertexMap[newHandle] = { mVertices.size(), model.vertices.size() };
	mIndexMap[newHandle] = { mIndices.size() - 1, model.indices.size() + 2 };

	std::transform(std::begin(model.indices), std::end(model.indices), std::back_inserter(mIndices),
		[indexStartPos](const VertexIndexType index)
		{
			return indexStartPos + index;
		});

	std::transform(std::begin(model.vertices), std::end(model.vertices), std::back_inserter(mVertices),
		[model2WorldTransform](Vertex vertex)
		{
			vertex.position = model2WorldTransform * vertex.position;
			return vertex;
		});

	if (sizeof(decltype(mVertices)::value_type) * mVertices.size() > std::numeric_limits<GLsizei>::max())
	{
		// TODO
		std::exit(-1);
	}

	return newHandle;
}


void BatchRenderer::ReplaceObject(const ObjectHandle objectHandle, const Model& model, const glm::mat4& model2WorldTransform)
{
	/* TODO: design decision
	 * - should it receive the model?
	 *	- if not, should we store them?
	 *		- if so, should we store it as a map and reference as a handle?
	 *		- if not, it maybe should be at the outside of the renderer
	 *
	 * - should we assume that the original and the new will be always have the same length of vertices & indices?
	 *	- if so, should we check?
	 *		- maybe only in Debug
	 *	- if not, would it be ok? since inserting in the middle / shrinking the vector is too slow
	 *		- even if it's ok, then there's no good in racking my brain to come up with the complicated algorithm
	 *		  instead of just removing and re-adding it
	 *			-> tested, wayyyyyyyyyyyyyyyy too slow
	 */
	
	
	RemoveObject(objectHandle);
	addObject(model, model2WorldTransform, objectHandle);
}


void BatchRenderer::ApplyMatrixToObject(const ObjectHandle objectHandle, const glm::mat4& model2WorldTransform)
{
	const auto vertexIt = mVertexMap.find(objectHandle);

	const bool bVertexNotFound = vertexIt == mVertexMap.end();
	const bool bIndexNotFound = mIndexMap.count(objectHandle) == 0;

#ifdef _DEBUG
	if (bVertexNotFound ^ bIndexNotFound)
	{
		std::exit(-1);
	}
#endif

	if (bVertexNotFound)
	{
		return;
	}

	const ObjectInfo info = vertexIt->second;
	const auto startIt = std::begin(mVertices) + info.index;
	std::for_each(startIt, startIt + info.size,
		[model2WorldTransform](Vertex& vertex)
		{
			vertex.position = model2WorldTransform * vertex.position;
		});
}


void BatchRenderer::RemoveObject(const ObjectHandle objectHandle)
{
	auto vertexIt = mVertexMap.find(objectHandle);
	auto indexIt = mIndexMap.find(objectHandle);

	const bool bVertexNotFound = vertexIt == mVertexMap.end();
	const bool bIndexNotFound = indexIt == mIndexMap.end();

#ifdef _DEBUG
	if (bVertexNotFound ^ bIndexNotFound)
	{
		std::exit(-1);
	}
#endif

	if (bVertexNotFound)
	{
		return;
	}

	const ObjectInfo vertexInfo = vertexIt->second;
	ObjectInfo indexInfo = indexIt->second;

	const auto vertexBegin = mVertices.begin() + vertexInfo.index;

	mVertices.erase(vertexBegin, vertexBegin + vertexInfo.size);
	std::for_each(++vertexIt, std::end(mVertexMap),
		[toSubtract = vertexInfo.size](auto& vertexMap)
	{
		vertexMap.second.index -= toSubtract;
	});

	mVertexMap.erase(objectHandle);

	const bool isFirstModel = indexInfo.index == static_cast<size_t>(-1);
	const bool isLastModel = (isFirstModel ? 0 : indexInfo.index) + indexInfo.size - 1 > mIndices.size();
	if (isFirstModel && isLastModel)
	{
		mIndexMap.clear();
		mIndices.clear();

		return;
	}

	if (isFirstModel)
	{
		indexInfo.index = 0;
	}
	else if (isLastModel)
	{
		indexInfo.index--;
	}

	const auto indexBegin = mIndices.begin() + indexInfo.index;

	const auto afterRemoveIt = mIndices.erase(indexBegin, indexBegin + indexInfo.size);
	std::for_each(++indexIt, std::end(mIndexMap),
		[toSubtract = indexInfo.size](auto& indexMap)
	{
		indexMap.second.index -= toSubtract;
	});
	std::for_each(afterRemoveIt, std::end(mIndices),
		[toSubtract = static_cast<VertexIndexType>(vertexInfo.size)](VertexIndexType& index)
	{
		index -= toSubtract;
	});

	mIndexMap.erase(objectHandle);
}


void BatchRenderer::Render(const Camera& camera) const
{
	static GLuint shaderProgramHandle = []()
	{
		const char* const vertexShader =
			"#version 450 core\n\n"

			"layout (location = 0) in vec4 inVertexPosition;\n"
			"layout (location = 1) in vec4 inVertexColor;\n"
			"layout (location = 2) in vec2 inVertexUV;\n"
			"layout (location = 3) in float inTextureIndex;\n\n"

			"uniform mat4 uWorld2NDC;\n\n"

			"layout (location = 0) out vec4 outVertexColor;\n"
			"layout (location = 1) out vec2 outVertexUV;\n"
			"layout (location = 2) out float outTextureIndex;\n\n"

			"void main()\n"
			"{\n\t"
				"gl_Position = uWorld2NDC * inVertexPosition;\n\t"
				"outVertexColor = inVertexColor;\n\t"
				"outVertexUV = inVertexUV;\n\t"
				"outTextureIndex = inTextureIndex;\n"
			"}\n"
			;
		const char* const fragmentShader =
			"#version 450 core\n\n"

			"layout (location = 0) in vec4 inVertexColor;\n"
			"layout (location = 1) in vec2 inVertexUV;\n"
			"layout (location = 2) in float inTextureIndex;\n\n"

			"uniform sampler2D uTextureUnits[1];\n\n"

			"layout (location = 0) out vec4 outFragmentColor;\n\n"

			"void main()\n"
			"{\n\t"
				"int inTextureIndex_ = int(inTextureIndex);\n\n\t"

				"if (inTextureIndex_ < 0)\n\t"
				"{\n\t\t"
					"outFragmentColor = inVertexColor;\n\t"
				"}\n\t"
				"else\n\t"
				"{\n\t\t"
					"outFragmentColor = texture(uTextureUnits[inTextureIndex_], inVertexUV);\n\t"
				"}\n"
			"}\n"
			;

		const GLuint vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderHandle, 1, &vertexShader, nullptr);
		glCompileShader(vertexShaderHandle);

		GLint compileStatus;
		glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE)
		{
			GLint log_len;
			glGetShaderiv(vertexShaderHandle, GL_INFO_LOG_LENGTH, &log_len);
			if (log_len > 0) {
				GLchar* log = new GLchar[log_len];
				GLsizei written_log_len;
				glGetShaderInfoLog(vertexShaderHandle, log_len, &written_log_len, log);
				std::cerr << "Error while compiling shader file: " << log << std::endl;
				delete[] log;
			}
		}

		const GLuint fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderHandle, 1, &fragmentShader, nullptr);
		glCompileShader(fragmentShaderHandle);
		glGetShaderiv(fragmentShaderHandle, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE)
		{
			GLint log_len;
			glGetShaderiv(fragmentShaderHandle, GL_INFO_LOG_LENGTH, &log_len);
			if (log_len > 0) {
				GLchar* log = new GLchar[log_len];
				GLsizei written_log_len;
				glGetShaderInfoLog(fragmentShaderHandle, log_len, &written_log_len, log);
				std::cerr << "Error while compiling shader file: " << log << std::endl;
				delete[] log;
			}
		}

		const GLuint shaderProgramHandle_ = glCreateProgram();
		glAttachShader(shaderProgramHandle_, vertexShaderHandle);
		glAttachShader(shaderProgramHandle_, fragmentShaderHandle);
		glLinkProgram(shaderProgramHandle_);
		GLint linkStatus;
		glGetProgramiv(shaderProgramHandle_, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == GL_FALSE)
		{
			GLint log_len;
			glGetProgramiv(shaderProgramHandle_, GL_INFO_LOG_LENGTH, &log_len);
			if (log_len > 0) {
				GLchar* log = new GLchar[log_len];
				GLsizei written_log_len;
				glGetProgramInfoLog(shaderProgramHandle_, log_len, &written_log_len, log);
				std::cerr << "Error while linking shader file: " << log << std::endl;
				delete[] log;
			}
		}
		glValidateProgram(shaderProgramHandle_);
		GLint validateStatus;
		glGetProgramiv(shaderProgramHandle_, GL_VALIDATE_STATUS, &validateStatus);
		if (validateStatus == GL_FALSE)
		{
			GLint log_len;
			glGetProgramiv(shaderProgramHandle_, GL_INFO_LOG_LENGTH, &log_len);
			if (log_len > 0) {
				GLchar* log = new GLchar[log_len];
				GLsizei written_log_len;
				glGetProgramInfoLog(shaderProgramHandle_, log_len, &written_log_len, log);
				std::cerr << "Error while validating shader file: " << log << std::endl;
				delete[] log;
			}
		}

		return shaderProgramHandle_;

		// TODO: delete shaders & program, error check
	}();
	
	glNamedBufferSubData(mVboHandle, 0, sizeof(decltype(mVertices)::value_type) * mVertices.size(), mVertices.data());
	glNamedBufferSubData(mEboHandle, 0, sizeof(decltype(mIndices)::value_type) * mIndices.size(), mIndices.data());

	glUseProgram(shaderProgramHandle);
	glBindVertexArray(mVaoHandle);

	const std::vector<GLuint>& textureHandles = mTextureSet.GetTextureHandles();
	const GLuint size = static_cast<GLuint>(textureHandles.size());
	for (GLuint i = 0; i < size; i++)
	{
		glBindTextureUnit(i, textureHandles.at(i));
	}

	const GLint uWorld2NDC = glGetUniformLocation(shaderProgramHandle, "uWorld2NDC");
	const GLint uTextureIndices = glGetUniformLocation(shaderProgramHandle, "uTextureUnits");

	glUniformMatrix4fv(uWorld2NDC, 1, GL_FALSE, glm::value_ptr(camera.GetWorld2NDCTransform()));
	
	std::vector<GLint> textureUnits(textureHandles.size());
	std::iota(std::begin(textureUnits), std::end(textureUnits), 0);
	glUniform1iv(uTextureIndices, static_cast<GLsizei>(textureUnits.size()), textureUnits.data());

	glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(mIndices.size()), get_opengl_type<VertexIndexType>(), nullptr);

	glBindVertexArray(0);
	glUseProgram(0);
}
