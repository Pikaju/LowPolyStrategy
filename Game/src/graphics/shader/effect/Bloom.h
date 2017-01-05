#pragma once

#include "../ShaderProgram.h"
#include "../../texture/Framebuffer.h"

namespace graphics {

	class Bloom
	{
	public:
		Bloom();
		~Bloom();

		void initialize();
		void cleanup();

		void setFramebufferSize(unsigned int width, unsigned int height);

		void render(const graphics::Framebuffer& source, unsigned int colorTextureID);

		inline const graphics::Framebuffer& getFramebuffer() const { return m_framebuffer; }
	private:
		graphics::ShaderProgram m_shader;
		graphics::Framebuffer m_framebuffer;
	};

}