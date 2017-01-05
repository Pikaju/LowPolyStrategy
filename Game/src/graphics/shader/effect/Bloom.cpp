#include "Bloom.h"

#include "../../renderer/AttributelessRenderer.h"

namespace graphics {

	Bloom::Bloom()
	{
	}

	Bloom::~Bloom()
	{
	}

	void Bloom::initialize()
	{
		m_shader.initialize();
		m_shader.loadShader("res/shaders/Bloom.vert", graphics::SHADERTYPE_VERTEX);
		m_shader.loadShader("res/shaders/Bloom.frag", graphics::SHADERTYPE_FRAGMENT);
		m_shader.link();
	}
	
	void Bloom::cleanup()
	{
		m_framebuffer.cleanup();
		m_shader.cleanup();
	}

	void Bloom::setFramebufferSize(unsigned int width, unsigned int height)
	{
		m_framebuffer.cleanup();
		std::vector<graphics::Attachment> attachments;
		attachments.push_back(graphics::ATTACHMENT_COLOR);
		std::vector<graphics::Format> formats;
		formats.push_back(graphics::FORMAT_RGB8);
		m_framebuffer.initialize(width, height, 1, attachments.data(), formats.data());
	}

	void Bloom::render(const graphics::Framebuffer& source, unsigned int colorTextureID)
	{
		graphics::AttributelessRenderer renderer;

		m_framebuffer.bind();
		renderer.clear(true, false);
		source.use(colorTextureID, 0);
		m_shader.enable();
		m_shader.setUniform("renderedColor", 0);
		renderer.renderTriangleStrip(4);
		m_shader.disable();
	}

}