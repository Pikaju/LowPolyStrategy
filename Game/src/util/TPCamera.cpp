#include "TPCamera.h"

#include "../io/Input.h"

namespace util {

	TPCamera::TPCamera(const math::Vec3f& origin, const math::Vec3f& rotation, float distance) : Camera(math::Vec3f(), rotation), origin(origin), distance(distance), distanceVelocity(0.0f)
	{
	}
	
	TPCamera::~TPCamera()
	{
	}

	void TPCamera::debugUpdate(const io::Input& input, float delta)
	{
		const float scrollSpeed = 1.0f / 512.0f;
		if (input.getState(io::MOUSE_BUTTON_MIDDLE)) {
			origin += math::Vec3f((float)input.getDX(), 0.0f, (float)input.getDY()) * -scrollSpeed * distance;
		}
		distanceVelocity -= (float)input.getDWheel() * 64.0f * delta;
		distance += distanceVelocity * delta * distance;
		distanceVelocity *= pow(1.0f / 32.0f, delta);
		calculatePosition();
	}

	void TPCamera::calculatePosition()
	{
		float rx = cos(rotation.x / 180.0f * 3.14159265f);
		position = origin + math::Vec3f(sin(rotation.y / 180.0f * 3.14159265f) * rx, sin(rotation.x / 180.0f * 3.14159265f), cos(rotation.y / 180.0f * 3.14159265f) * rx) * distance;
	}

	math::Mat4f TPCamera::getViewMatrix() const
	{
		return math::Mat4f::translation(-position.x, -position.y, -position.z) * math::Mat4f::rotationY(rotation.y) * math::Mat4f::rotationX(rotation.x) * math::Mat4f::rotationZ(rotation.z);
	}

}