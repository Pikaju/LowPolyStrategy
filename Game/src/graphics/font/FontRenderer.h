#pragma once

#include <vector>

#include "Font.h"
#include "../renderer/BatchRenderer.h"
#include "../shader/ShaderProgram.h"
#include "../../math/Vec2.h"
#include "../../math/Vec4.h"

namespace graphics {

	class FontVertex
	{
	public:
		math::Vec2f position;
		math::Vec2f texcoord;

		FontVertex() : position(), texcoord() {  }
		~FontVertex() {  }
	};

	class FontRenderer
	{
	public:
		FontRenderer();
		~FontRenderer();

		void intialize();
		void cleanup();

		void render(const char* text, int x, int y, float scale, unsigned int font = 0, const math::Vec4f& color = math::Vec4f(1.0f));

		inline void addFont(Font* font) { m_fonts.push_back(font); }
		inline Font* getFont(unsigned int index) { return m_fonts[index]; }
		inline void setScreenSize(const math::Vec2i& screenSize) { m_screenSize = screenSize; }
	private:
		std::vector<Font*> m_fonts;
		graphics::BatchRenderer m_renderer;
		graphics::ShaderProgram m_shader;
		math::Vec2i m_screenSize;
	};

}