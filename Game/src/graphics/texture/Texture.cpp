#include "Texture.h"

#include <glew.h>
#include <stb_image.h>

#include "../../debug/Debug.h"

namespace graphics {

	namespace format {

		unsigned int getGLInternalFormat(const Format& format)
		{
			switch (format) {
			case FORMAT_RGBA8: return GL_RGBA8;
			case FORMAT_RGB32F: return GL_RGB32F;
			case FORMAT_RGB8: return GL_RGB8;
			case FORMAT_R32F: return GL_R32F;
			case FORMAT_R8: return GL_R8;
			case FORMAT_DEPTH16: return GL_DEPTH_COMPONENT16;
			case FORMAT_DEPTH24: return GL_DEPTH_COMPONENT24;
			case FORMAT_DEPTH32: return GL_DEPTH_COMPONENT32;
			}
			return 0;
		}

		unsigned int getGLFormat(const Format& format)
		{
			switch (format) {
			case FORMAT_RGBA8: return GL_RGBA;
			case FORMAT_RGB32F: return GL_RGB;
			case FORMAT_RGB8: return GL_RGB;
			case FORMAT_R32F: return GL_RGBA;
			case FORMAT_R8: return GL_RGBA;
			case FORMAT_DEPTH16: return GL_DEPTH_COMPONENT;
			case FORMAT_DEPTH24: return GL_DEPTH_COMPONENT;
			case FORMAT_DEPTH32: return GL_DEPTH_COMPONENT;
			}
			return 0;
		}

		unsigned int getGLType(const Format& format)
		{
			switch (format) {
			case FORMAT_RGBA8: return GL_UNSIGNED_BYTE;
			case FORMAT_RGB32F: return GL_FLOAT;
			case FORMAT_RGB8: return GL_UNSIGNED_BYTE;
			case FORMAT_R32F: return GL_FLOAT;
			case FORMAT_R8: return GL_UNSIGNED_BYTE;
			case FORMAT_DEPTH16: return GL_FLOAT;
			case FORMAT_DEPTH24: return GL_FLOAT;
			case FORMAT_DEPTH32: return GL_FLOAT;
			}
			return 0;
		}

	}

	Texture::Texture() : m_id(0), m_width(0), m_height(0)
	{
	}

	Texture::~Texture()
	{
	}

	void Texture::initialize(unsigned int width, unsigned int height, const Format& format, const void* pixels)
	{
		m_width = width;
		m_height = height;
		if (m_id != 0) {
			use(0);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, format::getGLFormat(format), format::getGLType(format), pixels);
		}
		else {
			glGenTextures(1, &m_id);
			use(0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, format::getGLInternalFormat(format), m_width, m_height, 0, format::getGLFormat(format), format::getGLType(format), pixels);
		}
	}

	void Texture::initialize(const char* file)
	{
		int width;
		int height;
		int comp;
		unsigned char* pixels = stbi_load(file, &width, &height, &comp, 4);
		if (pixels == nullptr) {
			debug::crash(std::string("Could not find image file: \"") + file + "\"");
		}
		initialize((unsigned int)width, (unsigned int)height, FORMAT_RGBA8, pixels);
		stbi_image_free(pixels);
	}

	void Texture::cleanup()
	{
		glDeleteTextures(1, &m_id);
		m_id = 0;
	}

	void Texture::use(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

}