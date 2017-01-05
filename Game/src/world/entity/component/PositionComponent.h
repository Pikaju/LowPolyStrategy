#pragma once

#include "../../../math/Vec2.h"

namespace world {

	class PositionComponent
	{
	public:
		PositionComponent();
		~PositionComponent();

		void clientUpdate(float delta);

		inline const math::Vec2f& getPosition() const { return m_position; }
		inline void setPosition(const math::Vec2f& position) { m_targetPosition = position; }
	private:
		math::Vec2f m_position;
		math::Vec2f m_targetPosition;
	};

}