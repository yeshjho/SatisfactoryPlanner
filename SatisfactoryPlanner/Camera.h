#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>



class [[nodiscard]] Camera
{
public:
	constexpr Camera() noexcept = default;
	Camera(const glm::vec3& eye, const glm::vec3& look, const glm::vec3& up, float fov, float aspect, float near, float far) noexcept;

	constexpr Camera& Zoom(float factor) noexcept;
	
	constexpr Camera& MoveForward(float step) noexcept;
	constexpr Camera& MoveBackward(float step) noexcept;
	constexpr Camera& MoveRight(float step) noexcept;
	constexpr Camera& MoveLeft(float step) noexcept;

	Camera& Yaw(float angle) noexcept;
	Camera& Pitch(float angle) noexcept;
	Camera& Roll(float angle) noexcept;


	[[nodiscard]] glm::mat4 CalculateCam2WorldTransform() const noexcept;
	[[nodiscard]] glm::mat4 GetWorld2CamTransform() const noexcept;
	[[nodiscard]] glm::mat4 GetCam2NDCTransform() const noexcept;
	[[nodiscard]] glm::mat4 GetWorld2NDCTransform() const noexcept;



private:
	mutable bool mIsWorld2CamDirty = true;
	mutable bool mIsCam2NDCDirty = true;

	mutable glm::mat4 mWorld2CamTransform = glm::identity<glm::mat4>();
	mutable glm::mat4 mCam2NDCTransform = glm::identity<glm::mat4>();

	glm::vec3 mEye = { 0, 0, 0 };

	glm::vec3 mRight = { 1, 0, 0 };
	glm::vec3 mBack = { 0, 0, 1 };
	glm::vec3 mUp = { 0, 1, 0 };

	float mWidth = 0.2f;
	float mHeight = 0.2f;

	float mDistance = 0.1f;
	float mNear = 0.1f;
	float mFar = 10;
};



constexpr Camera& Camera::Zoom(const float factor) noexcept
{
	mWidth *= factor;
	mHeight *= factor;

	mIsCam2NDCDirty = true;

	return *this;
}


constexpr Camera& Camera::MoveForward(const float step) noexcept
{
	mEye += glm::vec3{ step * -mBack };

	mIsWorld2CamDirty = true;

	return *this;
}


constexpr Camera& Camera::MoveBackward(const float step) noexcept
{
	return MoveForward(-step);
}


constexpr Camera& Camera::MoveRight(const float step) noexcept
{
	mEye += glm::vec3{ step * mRight };

	mIsWorld2CamDirty = true;

	return *this;
}


constexpr Camera& Camera::MoveLeft(const float step) noexcept
{
	return MoveRight(-step);
}
