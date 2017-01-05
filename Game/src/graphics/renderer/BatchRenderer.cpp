#include "BatchRenderer.h"

#include <glew.h>
#include <cstring>

#include "../Attribute.h"

namespace graphics {

	BatchRenderer::BatchRenderer() : m_vbo(0), m_vao(0), m_index(0), m_mode(GL_TRIANGLES), m_numAllocatedVertices(0), m_pointer(nullptr)
	{
	}

	BatchRenderer::~BatchRenderer()
	{
	}

	void BatchRenderer::initialize(unsigned int numAllocatedVertices, const Attribute* attributes, unsigned int numAttributes)
	{
		glGenBuffers(1, &m_vbo);
		glGenVertexArrays(1, &m_vao);

		m_numAllocatedVertices = numAllocatedVertices;

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		m_stride = 0;
		for (unsigned int i = 0; i < numAttributes; i++) {
			m_stride += attribute::getGLTypeSize(attributes[i].type) * attributes[i].amount;
		}
		glBufferData(GL_ARRAY_BUFFER, m_stride * m_numAllocatedVertices, nullptr, GL_DYNAMIC_DRAW);

		unsigned int pointer = 0;
		for (unsigned int i = 0; i < numAttributes; i++) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, attributes[i].amount, attribute::getGLType(attributes[i].type), attributes[i].normalized, m_stride, (const void*)pointer);
			pointer += attribute::getGLTypeSize(attributes[i].type) * attributes[i].amount;
		}

		glBindVertexArray(0);
	}
	
	void BatchRenderer::cleanup()
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}

	void BatchRenderer::begin()
	{
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		m_pointer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}
	
	void BatchRenderer::end()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
		flush();
		m_pointer = nullptr;
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void BatchRenderer::flush()
	{
		glDrawArrays(m_mode, 0, m_index);
		m_index = 0;
	}

	void BatchRenderer::submit(const void* vertex)
	{
		memcpy((unsigned char*)m_pointer + (m_index * m_stride), vertex, m_stride);
		m_index++;
		if (m_index >= m_numAllocatedVertices) {
			end();
			begin();
		}
	}

}