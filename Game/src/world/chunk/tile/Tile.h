#pragma once

#include "../../../math/Vec3.h"

namespace world {

	class Tile
	{
	public:
		Tile();
		~Tile();
	
		inline float getHeight() const { return m_height; }
		inline void setHeight(float height) { m_height = height; }

		inline const math::Vec3b& getColor() const { return m_color; }
		inline void setColor(const math::Vec3b& color) { m_color = color; }

		//inline int getFOW() const { return m_fow < 0 ? 128 : (m_fow > 0 ? 255 : 172); }
		inline int getFOW() const { return m_fow < 0 ? 0 : (m_fow > 0 ? 255 : 128); }
		inline void increaseFOW() { if (m_fow < 0) m_fow = 0; m_fow++; }
		inline void decreaseFOW() { m_fow--; }
	private:
		float m_height;
		math::Vec3b m_color;
		int m_fow;
	};

}