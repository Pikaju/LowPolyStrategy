#include "Button.h"

namespace graphics {

	Button::Button(const char* text, int x, int y, int width, int height, float scale) : m_text(text), m_x(x), m_y(y), m_width(width), m_height(height), m_scale(scale)
	{
	}

	Button::~Button()
	{
	}

	void Button::render(graphics::FontRenderer& fontRenderer)
	{
		fontRenderer.render(m_text, m_x + m_width / 2 - fontRenderer.getFont(0)->getStringWidth(m_text) / 2 * m_scale, m_y, m_scale);
	}

}