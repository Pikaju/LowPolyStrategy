#pragma once

#include "../font/FontRenderer.h"

namespace graphics {

	class Button
	{
	public:
		Button(const char* text, int x, int y, int width, int height, float scale);
		~Button();

		void render(graphics::FontRenderer& fontRenderer);
	private:
		const char* m_text;
		int m_x;
		int m_y;
		int m_width;
		int m_height;
		float m_scale;
	};

}