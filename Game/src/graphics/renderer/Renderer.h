#pragma once

namespace graphics {

	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		void setDepthRead(bool depth) const;
		void setDepthWrite(bool depth) const;
		void setCulling(bool culling) const;
		void setWireframe(bool wireframe) const;
		void setBlending(bool blending, bool additive) const;
		void clear(bool color = true, bool depth = false, bool stencil = false) const;
	};

}