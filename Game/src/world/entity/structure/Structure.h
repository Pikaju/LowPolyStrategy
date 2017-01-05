#pragma once

#include "../Entity.h"

namespace world {

	class Structure : public Entity
	{
	public:
		Structure();
		~Structure();

		inline void setPosition(int x, int y) { m_x = x; m_y = y; }
	protected:
		int m_x;
		int m_y;
		int m_width;
		int m_height;
	};

}