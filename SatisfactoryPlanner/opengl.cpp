#include <functional>
#include <queue>

#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

#include "BatchRenderer.h"
#include "Camera.h"
#include "Model.h"
#include "Window.h"




// temp for not being distracted by fucking glfw key binding
std::vector<std::function<void(int, int, int, int)>> key_callbacks;
void key_callback(GLFWwindow*, const int key, [[maybe_unused]] int scanCode, const int action, [[maybe_unused]] int mods)
{
	for (auto& f : key_callbacks)
	{
		f(key, scanCode, action, mods);
	}
}



glm::mat4 rotate(const float xAngle, const float yAngle, const float zAngle)
{
	const float cosX = cos(xAngle);
	const float cosY = cos(yAngle);
	const float cosZ = cos(zAngle);
	const float sinX = sin(xAngle);
	const float sinY = sin(yAngle);
	const float sinZ = sin(zAngle);
	
	return {
		cosZ * cosY, sinZ * cosY, -sinY, 0,
		cosZ * sinY * sinX - sinZ * cosX, sinZ * sinY * sinX + cosZ * cosX, cosY * sinX, 0,
		cosZ * sinY * cosX + sinZ * sinX, sinZ * sinY * cosX - cosZ * sinX, cosY * cosX, 0,
		0, 0, 0, 1
	};
}


int main()
{
	const Window window = create_window();
	BatchRenderer renderer{ "resource/texture/textures.yaml" };
	Camera camera{ glm::vec3{ 0, 0, 9 }, glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 },
		glm::pi<float>() / 2, 16.f / 9.f, 0.1f, 10 };
	//camera.Zoom(100.f);


	glfwSetKeyCallback(window.GetWindow(), key_callback);
	key_callbacks.emplace_back(
		[&renderer, handleQueue = std::queue<ObjectHandle>{}](const int key, [[maybe_unused]] int scanCode, const int action, [[maybe_unused]] int mods) mutable
		{
			static const TextureSet::TextureInfo model1TextureInfo = renderer.GetTextureSet().GetTextureInfo("first_texture");
			static const TextureSet::TextureInfo::Texel model1Texel = model1TextureInfo.texel;
			static const TextureSet::TextureInfo model2TextureInfo = renderer.GetTextureSet().GetTextureInfo("second_texture");
			static const TextureSet::TextureInfo::Texel model2Texel = model2TextureInfo.texel;
			static const Model model1{
				{
					{ { 0, 1, 0, 1 }, { 0, 1, 0, 1 }, { model1Texel.x, model1Texel.y + model1Texel.height }, model1TextureInfo.textureIndex },
					{ { 0, 0, 0, 1 }, { 1, 0, 0, 1 }, { model1Texel.x, model1Texel.y }, model1TextureInfo.textureIndex },
					{ { 1, 1, 0, 1 }, { 0, 0, 1, 1 }, { model1Texel.x + model1Texel.width, model1Texel.y + model1Texel.height }, model1TextureInfo.textureIndex },
					{ { 1, 0, 0, 1 }, { 1, 1, 1, 1 }, { model1Texel.x + model1Texel.width, model1Texel.y }, model1TextureInfo.textureIndex },
				},
				{ 0, 1, 2, 3 }
			};
			static const Model model2{
				{
					{ { 0, 1, 0, 1 }, { 0, 1, 0, 1 }, { model2Texel.x, model2Texel.y + model2Texel.height }, model2TextureInfo.textureIndex },
					{ { 0, 0, 0, 1 }, { 1, 0, 0, 1 }, { model2Texel.x, model2Texel.y }, model2TextureInfo.textureIndex },
					{ { 1, 1, 0, 1 }, { 0, 0, 1, 1 }, { model2Texel.x + model2Texel.width, model2Texel.y + model2Texel.height }, model2TextureInfo.textureIndex },
					{ { 1, 0, 0, 1 }, { 1, 1, 1, 1 }, { model2Texel.x + model2Texel.width, model2Texel.y }, model2TextureInfo.textureIndex },
				},
				{ 0, 1, 2, 3 }
			};
			
			if (action != GLFW_RELEASE)
			{
				return;
			}
			
			static float x = -1.5f;
			static float y = 0;
			switch (key)
			{
				case GLFW_KEY_1:
					handleQueue.emplace(renderer.AddObject(model1, glm::translate(glm::vec3{ x += 1.5f, 0, 0 }) * rotate(0, 0, 0) * glm::scale(glm::vec3{ 1, 1, 1 })));
					break;
				
				case GLFW_KEY_2:
					handleQueue.emplace(renderer.AddObject(model2, glm::translate(glm::vec3{ 1.5f, y += 1.5f, 0 }) * rotate(0, 0, 0) * glm::scale(glm::vec3{ 1, 1, 1 })));
					break;

				case GLFW_KEY_0:
					renderer.RemoveObject(handleQueue.front());
					handleQueue.pop();
					break;

				default:
					break;
			}
		});
	

	while (!window.ShouldClose())
	{
		window.SwapBuffers();
		glClear(GL_COLOR_BUFFER_BIT/* | GL_DEPTH_BUFFER_BIT)*/);
		glfwPollEvents();

		renderer.Render(camera);
	}
}
