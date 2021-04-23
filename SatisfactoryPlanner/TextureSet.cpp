#include "TextureSet.h"

#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION
#include <yaml-cpp/yaml.h>



TextureSet::TextureSet(const std::filesystem::path& filePath)
{
	YAML::Node file = YAML::LoadFile(filePath.string());
	mTextureHandles.reserve(file.size());

	for (const auto& imagePair : file)
	{
		YAML::Node textureInfo = imagePair.second;
		const std::string imageFilePath = textureInfo["imageFilePath"].as<std::string>();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* const data = stbi_load(imageFilePath.c_str(), &width, &height, &channels, 0);
		if (data == nullptr)
		{
			std::exit(-1);
		}

		GLuint textureHandle;
		glCreateTextures(GL_TEXTURE_2D, 1, &textureHandle);
		glTextureStorage2D(textureHandle, 1, GL_RGBA8, width, height);
		glTextureSubImage2D(textureHandle, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

		glTextureParameteri(textureHandle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(textureHandle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(textureHandle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(textureHandle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		for (const auto& texturePair : textureInfo["textures"])
		{
			const std::string textureName = texturePair.first.as<std::string>();
			YAML::Node texel = texturePair.second;
			mTextureInfoByTextureName[textureName] = {
				static_cast<TextureIndexType>(mTextureHandles.size()),
				{
				texel[0].as<float>() / static_cast<float>(width),
				texel[1].as<float>() / static_cast<float>(height),
				texel[2].as<float>() / static_cast<float>(width),
				texel[3].as<float>() / static_cast<float>(height)
			} };
		}

		mTextureHandles.push_back(textureHandle);
	}
}


TextureSet::~TextureSet()
{
	glDeleteTextures(static_cast<GLsizei>(mTextureHandles.size()), mTextureHandles.data());
}


TextureSet::TextureInfo TextureSet::GetTextureInfo(const std::string& textureName) const
{
	return mTextureInfoByTextureName.at(textureName);
}
