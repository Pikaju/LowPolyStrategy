#include "Water.h"

#include "../../graphics/model/Mesh.h"
#include "../../math/Vec3.h"

namespace world {

	Water::Water()
	{
	}

	Water::~Water()
	{
	}

	void Water::initialize(unsigned int size, float scale)
	{
		m_shader.initialize();
		m_shader.loadShader("res/shaders/Water.vert", graphics::SHADERTYPE_VERTEX);
		m_shader.loadShader("res/shaders/Water.frag", graphics::SHADERTYPE_FRAGMENT);
		m_shader.link();
	
		m_model.initialize();
		graphics::Mesh<math::Vec3f, unsigned int> mesh;
		mesh.addAttribute(graphics::Attribute(graphics::TYPE_FLOAT, 3));
		for (unsigned int x = 0; x < size; x++) {
			for (unsigned int y = 0; y < size; y++) {
				math::Vec3f v0;
				math::Vec3f v1;
				math::Vec3f v2;
				math::Vec3f v3;

				if ((x + y) % 2 == 0) {
					v0 = math::Vec3f((float)x * scale, 0.0f, (float)y * scale);
					v1 = math::Vec3f((float)x * scale, 0.0f, (float)(y + 1) * scale);
					v2 = math::Vec3f((float)(x + 1) * scale, 0.0f, (float)(y + 1) * scale);
					v3 = math::Vec3f((float)(x + 1) * scale, 0.0f, (float)y * scale);
				}
				else {
					v0 = math::Vec3f((float)x * scale, 0.0f, (float)(y + 1) * scale);
					v1 = math::Vec3f((float)(x + 1) * scale, 0.0f, (float)(y + 1) * scale);
					v2 = math::Vec3f((float)(x + 1) * scale, 0.0f, (float)y * scale);
					v3 = math::Vec3f((float)x * scale, 0.0f, (float)y * scale);
				}

				mesh.triangle(v0, v1, v2);
				mesh.triangle(v0, v2, v3);
			}
		}
		mesh.createModel(m_model);
	}
	
	void Water::cleanup()
	{
		m_model.cleanup();
		m_shader.cleanup();
	}

	void Water::update(float delta)
	{
		m_time += delta;
	}

	void Water::render()
	{
		m_model.render();
	}

	void Water::updateUniforms()
	{
		m_shader.setUniform("time", m_time);
	}

}