#include <chrono>
#include <iostream>
#include <random>
#include <thread>

#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

#include "BatchRenderer.h"
#include "Camera.h"
#include "InputCallback.h"
#include "Model.h"
#include "Window.h"


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
		glm::pi<float>() / 2, 16.f / 9.f, 0.1f, 100000 };

	std::vector<ObjectHandle> handles;
	handles.reserve(317 * 317);
	const TextureSet::TextureInfo model1TextureInfo = renderer.GetTextureSet().GetTextureInfo("first_texture");
	const TextureSet::TextureInfo::Texel model1Texel = model1TextureInfo.texel;
	const TextureSet::TextureInfo model2TextureInfo = renderer.GetTextureSet().GetTextureInfo("second_texture");
	const TextureSet::TextureInfo::Texel model2Texel = model2TextureInfo.texel;
	const Model model1{
		{
			{ { 0, 1, 0, 1 }, { 0, 1, 0, 1 }, { model1Texel.x, model1Texel.y + model1Texel.height }, model1TextureInfo.textureIndex },
			{ { 0, 0, 0, 1 }, { 1, 0, 0, 1 }, { model1Texel.x, model1Texel.y }, model1TextureInfo.textureIndex },
			{ { 1, 1, 0, 1 }, { 0, 0, 1, 1 }, { model1Texel.x + model1Texel.width, model1Texel.y + model1Texel.height }, model1TextureInfo.textureIndex },
			{ { 1, 0, 0, 1 }, { 1, 1, 1, 1 }, { model1Texel.x + model1Texel.width, model1Texel.y }, model1TextureInfo.textureIndex },
		},
		{ 0, 1, 2, 3 }
	};
	const Model model2{
		{
			{ { 0, 1, 0, 1 }, { 0, 1, 0, 1 }, { model2Texel.x, model2Texel.y + model2Texel.height }, model2TextureInfo.textureIndex },
			{ { 0, 0, 0, 1 }, { 1, 0, 0, 1 }, { model2Texel.x, model2Texel.y }, model2TextureInfo.textureIndex },
			{ { 1, 1, 0, 1 }, { 0, 0, 1, 1 }, { model2Texel.x + model2Texel.width, model2Texel.y + model2Texel.height }, model2TextureInfo.textureIndex },
			{ { 1, 0, 0, 1 }, { 1, 1, 1, 1 }, { model2Texel.x + model2Texel.width, model2Texel.y }, model2TextureInfo.textureIndex },
		},
		{ 0, 1, 2, 3 }
	};
	float x = -0.5f;
	float z = 0.5f;
	float y = 0;

	const EMouseButton cameraOperateButton = EMouseButton::RIGHT;

	InputCallback camRotationCallback{ EInputType::MOUSE_MOVE,
		[&camera, cameraOperateButton](const float moveX, const float moveY)
		{
			if (InputManager::IsMouseDown(cameraOperateButton))
			{
				camera.Yaw(-static_cast<float>(moveX) * glm::pi<float>() / 180 / 2)
					.Pitch(-static_cast<float>(moveY) * glm::pi<float>() / 180 / 2);
			}
		}
	};
	InputCallback camZoomCallback{
		EInputType::MOUSE_SCROLL,
		[&camera](float, const float yMove)
		{
			if (yMove < 0)
			{
				camera.Zoom(-yMove * 1.5f);
			}
			else
			{
				camera.Zoom(1 / yMove / 2);
			}
		}
	};
	InputCallback camRollClockwiseCallback{
		[&camera, cameraOperateButton]()
		{
			if (InputManager::IsMouseDown(cameraOperateButton))
			{
				camera.Roll(glm::pi<float>() / 180 / 2);
			}
		},
		EKeyButton::E, EButtonAction::REPEAT
	};
	InputCallback camRollCounterClockwiseCallback{
		[&camera, cameraOperateButton]()
		{
			if (InputManager::IsMouseDown(cameraOperateButton))
			{
				camera.Roll(-glm::pi<float>() / 180 / 2);
			}
		},
		EKeyButton::Q, EButtonAction::REPEAT
	};
	InputCallback camForwardCallback {
		[&camera, cameraOperateButton]()
		{
			if (InputManager::IsMouseDown(cameraOperateButton))
			{
				camera.MoveForward(1.f);
			}
		},
		EKeyButton::W, EButtonAction::REPEAT
	};
	InputCallback camBackwardCallback{
		[&camera, cameraOperateButton]()
		{
			if (InputManager::IsMouseDown(cameraOperateButton))
			{
				camera.MoveBackward(1.f);
			}
		},
		EKeyButton::S, EButtonAction::REPEAT
	};
	InputCallback camRightCallback{
		[&camera, cameraOperateButton]()
		{
			if (InputManager::IsMouseDown(cameraOperateButton))
			{
				camera.MoveRight(1.f);
			}
		},
		EKeyButton::D, EButtonAction::REPEAT
	};
	InputCallback camLeftCallback{
		[&camera, cameraOperateButton]()
		{
			if (InputManager::IsMouseDown(cameraOperateButton))
			{
				camera.MoveLeft(1.f);
			}
		},
		EKeyButton::A, EButtonAction::REPEAT
	};
	
	InputCallback addFirstModelCallback{
		[&]()
		{
			handles.emplace_back(renderer.AddObject(model1, glm::translate(glm::vec3{ x += 0.5f, 0, z -= 0.5f }) * rotate(0, 0, 0) * glm::scale(glm::vec3{ 1, 1, 1 })));
		},
		EKeyButton::NUM_1
	};
	InputCallback addSecondModelCallback{
		[&]()
		{
			handles.emplace_back(renderer.AddObject(model2, glm::translate(glm::vec3{ 1.5f, y += 1.5f, 0 }) * rotate(0, 0, 0) * glm::scale(glm::vec3{ 1, 1, 1 })));
		},
		EKeyButton::NUM_2
	};
	//InputCallback removeModelCallback{
	//	[&]()
	//	{
	//		renderer.RemoveObject(handleQueue.front());
	//		handleQueue.pop();
	//	},
	//	EKeyButton::NUM_0
	//};

	for (int i = 0; i < 317; i++)
	{
		for (int j = 0; j < 317; j++)
		{
			handles.emplace_back(renderer.AddObject(model1, 
				glm::translate(glm::vec3{ i, j, 0 }) * 
				rotate(0, 0, 0) * 
				glm::scale(glm::vec3{ 1, 1, 1 })));
		}
	}

	std::atomic<bool> shouldEnd = false;

	std::thread t{
		[&shouldEnd, handles, &renderer]()
		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(5s);
			
			std::random_device rd{};
			const std::uniform_real_distribution<float> locDist{ -3.f / 144, 3.f / 144 };
			const std::uniform_real_distribution<float> angleDist{ -glm::pi<float>() / 2, glm::pi<float>() / 2 };
			const std::uniform_real_distribution<float> scaleDist{ 0.5f, 2.f };
			
			while (!shouldEnd)
			{
				for (int i = 0; i < 317; i++)
				{
					for (int j = 0; j < 317; j++)
					{
						const ObjectHandle handle = handles[i * 317 + j];
						renderer.ApplyMatrixToObject(handle,
							/*glm::translate(glm::vec3{ locDist(rd), locDist(rd), 0 }) **/
							glm::translate(glm::vec3{ i, j, 0 }) *
							rotate(angleDist(rd), angleDist(rd), angleDist(rd)) *
							glm::scale(glm::vec3{ scaleDist(rd), scaleDist(rd), 1 })*
							glm::translate(glm::vec3{ -i, -j, 0 })
							);
					}
				}
			}
		}
	};
	

	while (!window.ShouldClose())
	{
		using namespace std::chrono;
		
		static time_point<steady_clock> previous = steady_clock::now();
		const time_point<steady_clock> current = steady_clock::now();
		const long long deltaTimeInNanosecond = duration_cast<nanoseconds>(current - previous).count();
		previous = current;

		std::cout << "FPS: " << 1'000'000'000 / static_cast<double>(deltaTimeInNanosecond) << '\n';
		
		
		window.SwapBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		InputManager::_Update();

		renderer.Render(camera);
	}

	shouldEnd = true;
	t.join();
}
