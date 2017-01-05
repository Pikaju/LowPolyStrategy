#pragma once

#include "Camera.h"

namespace util {

	class TPCamera : public Camera
	{
	public:
		math::Vec3f origin;
		float distance;
		float distanceVelocity;

		TPCamera(const math::Vec3f& origin = math::Vec3f(), const math::Vec3f& rotation = math::Vec3f(), float distance = 1.0f);
		~TPCamera();

		void debugUpdate(const io::Input& input, float delta) override;

		void calculatePosition();
		math::Mat4f getViewMatrix() const override;
	};

}