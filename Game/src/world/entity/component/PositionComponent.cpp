#include "PositionComponent.h"

namespace world {

	PositionComponent::PositionComponent() : m_position(), m_targetPosition()
	{
	}

	PositionComponent::~PositionComponent()
	{
	}

	void PositionComponent::clientUpdate(float delta)
	{
		m_position = m_targetPosition;
	}

}