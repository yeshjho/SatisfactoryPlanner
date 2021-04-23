#pragma once
#include <filesystem>
#include <unordered_map>

#include "Common.h"



class [[nodiscard]] TextureSet
{
public:
	struct [[nodiscard]] TextureInfo
	{
		TextureIndexType textureIndex;
		struct Texel
		{
			float x, y, width, height;
		} texel;
	};

public:
	TextureSet(const std::filesystem::path& filePath);
	~TextureSet();

	TextureSet(const TextureSet&) = delete;
	TextureSet& operator=(const TextureSet&) = delete;

	TextureSet(TextureSet&&) noexcept = default;
	TextureSet& operator=(TextureSet&&) noexcept = default;


	[[nodiscard]] TextureInfo GetTextureInfo(const std::string& textureName) const;
	[[nodiscard]] constexpr const std::vector<unsigned int>& GetTextureHandles() const noexcept { return mTextureHandles; }


private:
	std::unordered_map<std::string, TextureInfo> mTextureInfoByTextureName;
	std::vector<unsigned int> mTextureHandles;
};

