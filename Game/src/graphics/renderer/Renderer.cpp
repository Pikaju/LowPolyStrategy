#include "Renderer.h"

#include <glew.h>

#include "../../io/Log.h"

namespace graphics {

	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::setDepthRead(bool depth) const
	{
		if (depth) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);
	}

	void Renderer::setDepthWrite(bool depth) const
	{
		glDepthMask(depth);
	}

	void Renderer::setCulling(bool culling) const
	{
		if (culling) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);
	}

	void Renderer::setWireframe(bool wireframe) const
	{
		if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void Renderer::setBlending(bool blending, bool additive) const
	{
		if (blending) {
			glEnablei(GL_BLEND, 0);
			if (additive) glBlendFunc(GL_ONE, GL_ONE);
			else glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else glDisable(GL_BLEND);
	}

	void Renderer::clear(bool color, bool depth, bool stencil) const
	{
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		int flags = 0;
		if (color) flags |= GL_COLOR_BUFFER_BIT;
		if (depth) flags |= GL_DEPTH_BUFFER_BIT;
		if (stencil) flags |= GL_STENCIL_BUFFER_BIT;
		glClear(flags);
	}

}