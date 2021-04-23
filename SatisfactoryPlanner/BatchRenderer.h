#pragma once
#include <filesystem>
#include <map>

#include <glm/ext/matrix_transform.hpp>

#include "Common.h"
#include "TextureSet.h"
#include "Vertex.h"



class Camera;
struct Model;



class [[nodiscard]] BatchRenderer
{
public:
	BatchRenderer(const std::filesystem::path& textureSetFilePath);
	~BatchRenderer();

	BatchRenderer(const BatchRenderer&) = delete;
	BatchRenderer& operator=(const BatchRenderer&) = delete;

	BatchRenderer(BatchRenderer&&) noexcept = default;
	BatchRenderer& operator=(BatchRenderer&&) noexcept = default;


	// TODO: Objects are static for now
	[[nodiscard]] ObjectHandle AddObject(const Model& model, const glm::mat4& model2WorldTransform = glm::identity<glm::mat4>());
	void RemoveObject(ObjectHandle objectHandle);

	void Render(const Camera& camera) const;

	[[nodiscard]] constexpr TextureSet& GetTextureSet() noexcept { return mTextureSet; }
	[[nodiscard]] constexpr const TextureSet& GetTextureSet() const noexcept { return mTextureSet; }



private:
	struct [[nodiscard]] ObjectInfo
	{
		size_t index;
		size_t size;
	};

private:
	static constexpr size_t INITIAL_VERTEX_COUNT = 4 * 1000;
	static constexpr size_t INITIAL_INDEX_COUNT = 4 * 2 * 1000;

	unsigned int mVboHandle;
	unsigned int mVaoHandle;
	unsigned int mEboHandle;

	std::vector<Vertex> mVertices;
	std::vector<VertexIndexType> mIndices;

	std::map<ObjectHandle, ObjectInfo> mVertexMap;
	std::map<ObjectHandle, ObjectInfo> mIndexMap;

	ObjectHandle mNextObjectHandle = 0;

	// TODO: is this the right thing?
	TextureSet mTextureSet;
};

