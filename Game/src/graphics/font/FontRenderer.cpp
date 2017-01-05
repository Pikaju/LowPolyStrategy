#include "FontRenderer.h"

#include "../Attribute.h"
#include "../../math/Mat4.h"

namespace graphics {

	FontRenderer::FontRenderer()
	{
	}
	
	FontRenderer::~FontRenderer()
	{
	}

	void FontRenderer::intialize()
	{
		std::vector<graphics::Attribute> attributes;
		attributes.push_back(graphics::Attribute(graphics::TYPE_FLOAT, 2));
		attributes.push_back(graphics::Attribute(graphics::TYPE_FLOAT, 2));
		m_renderer.initialize(6 * 128, attributes.data(), attributes.size());
		
		m_shader.initialize();
		m_shader.loadShader("res/shaders/Font.vert", graphics::SHADERTYPE_VERTEX);
		m_shader.loadShader("res/shaders/Font.frag", graphics::SHADERTYPE_FRAGMENT);
		m_shader.link();
	}
	
	void FontRenderer::cleanup()
	{
		for (unsigned int i = 0; i < m_fonts.size(); i++) {
			m_fonts[i]->cleanup();
			delete m_fonts[i];
		}
		m_fonts.clear();

		m_shader.cleanup();
		m_renderer.cleanup();
	}

	void FontRenderer::render(const char* text, int x, int y, float scale, unsigned int font, const math::Vec4f& color)
	{
		char c;
		int index = -1;
		FontVertex v0;
		FontVertex v1;
		FontVertex v2;
		FontVertex v3;

		math::Vec2f textureSize = math::Vec2f((float)m_fonts[font]->getTexture().getWidth(), (float)m_fonts[font]->getTexture().getHeight());

		int xAdvance = 0;

		m_fonts[font]->getTexture().use(0);
		m_shader.enable();
		m_shader.setUniform("projectionMatrix", math::Mat4f::ortho(0.0f, (float)m_screenSize.x, (float)m_screenSize.y, 0.0f, -1.0f, 1.0f));
		m_shader.setUniform("fontTexture", 0);
		m_shader.setUniform("scale", scale);
		m_shader.setUniform("color", color);

		m_renderer.setDepthRead(false);
		m_renderer.setBlending(true, false);
		m_renderer.begin();
		
		while (true) {
			index++;
			c = text[index];
			if (c == '\0') {
				break;
			}

			const Character& ci = m_fonts[font]->getCharacter(c);
			
			float cx = (float)x + (float)xAdvance * scale + (float)ci.xOffset * scale;
			float cy = (float)y + (float)ci.yOffset * scale;
			float cw = (float)ci.width * scale;
			float ch = (float)ci.height * scale;

			v0.position = math::Vec2f(cx, cy);
			v0.texcoord = math::Vec2f((float)ci.x, (float)ci.y) / textureSize;
			v1.position = math::Vec2f(cx, cy + ch);
			v1.texcoord = math::Vec2f((float)ci.x, (float)(ci.y + ci.height)) / textureSize;
			v2.position = math::Vec2f(cx + cw, cy + ch);
			v2.texcoord = math::Vec2f((float)(ci.x + ci.width), (float)(ci.y + ci.height)) / textureSize;
			v3.position = math::Vec2f(cx + cw, cy);
			v3.texcoord = math::Vec2f((float)(ci.x + ci.width), (float)ci.y) / textureSize;
			
			m_renderer.submit(&v0);
			m_renderer.submit(&v1);
			m_renderer.submit(&v2);

			m_renderer.submit(&v0);
			m_renderer.submit(&v2);
			m_renderer.submit(&v3);

			xAdvance += ci.xAdvance - 8;
		}

		m_renderer.end();
		m_renderer.setDepthRead(true);

		m_shader.disable();
	}

}