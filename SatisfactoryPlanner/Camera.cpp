#include "Camera.h"

#include <glm/gtx/transform.hpp>



Camera::Camera(const glm::vec3& eye, const glm::vec3& look, const glm::vec3& up,
	const float fov, const float aspect, const float near, const float far) noexcept
	: mEye(eye), mRight(glm::normalize(glm::cross(look, up))), mBack(-glm::normalize(look)), mUp(glm::cross(mBack, mRight)),
	mWidth(2 * near * std::tan(fov / 2)), mHeight(mWidth / aspect), mDistance(near), mNear(near), mFar(far)
{}


Camera& Camera::Yaw(const float angle) noexcept
{
	const glm::mat4 rotationMatrix = glm::rotate(angle, /*mUp*/glm::vec3{ 0, 1, 0 });

	mRight = rotationMatrix * glm::vec4{ mRight, 0 };
	mBack = rotationMatrix * glm::vec4{ mBack, 0 };

	mIsWorld2CamDirty = true;

	return *this;
}


Camera& Camera::Pitch(const float angle) noexcept
{
	const glm::mat4 rotationMatrix = glm::rotate(angle, mRight);

	//mUp = rotationMatrix * glm::vec4{ mUp, 0 };
	mBack = rotationMatrix * glm::vec4{ mBack, 0 };
	mUp = glm::cross(mBack, mRight);

	mIsWorld2CamDirty = true;

	return *this;
}


Camera& Camera::Roll(const float angle) noexcept
{
	const glm::mat4 rotationMatrix = glm::rotate(angle, mBack);

	mUp = rotationMatrix * glm::vec4{ mUp, 0 };
	mRight = rotationMatrix * glm::vec4{ mRight, 0 };

	mIsWorld2CamDirty = true;

	return *this;
}


glm::mat4 Camera::CalculateCam2WorldTransform() const noexcept
{
	return {
		glm::vec4{ mRight, 0 },
		glm::vec4{ mUp, 0 },
		glm::vec4{ mBack, 0 },
		glm::vec4{ mEye, 1 }
	};
}


glm::mat4 Camera::GetWorld2CamTransform() const noexcept
{
	if (mIsWorld2CamDirty)
	{
		mWorld2CamTransform = glm::inverse(CalculateCam2WorldTransform());

		mIsWorld2CamDirty = false;
	}

	return mWorld2CamTransform;
}


glm::mat4 Camera::GetCam2NDCTransform() const noexcept
{
	if (mIsCam2NDCDirty)
	{
		const float nfDiff = mNear - mFar;
		const float doubleD = 2 * mDistance;

		mCam2NDCTransform = {
			glm::vec4{ doubleD / mWidth, 0, 0, 0 },
			glm::vec4{ 0, doubleD / mHeight, 0, 0 },
			glm::vec4{ 0, 0, (mNear + mFar) / nfDiff, -1 },
			glm::vec4{ 0, 0, 2 * mNear * mFar / nfDiff, 0 }
		};

		mIsCam2NDCDirty = false;
	}

	return mCam2NDCTransform;
}


glm::mat4 Camera::GetWorld2NDCTransform() const noexcept
{
	return GetCam2NDCTransform() * GetWorld2CamTransform();
}
