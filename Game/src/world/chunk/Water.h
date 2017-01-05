#pragma once

#include "../../graphics/model/Model.h"
#include "../../graphics/shader/ShaderProgram.h"

namespace world {

	class Water
	{
	public:
		Water();
		~Water();

		void initialize(unsigned int size, float scale);
		void cleanup();

		void update(float delta);
		void render();

		void updateUniforms();
		inline const graphics::ShaderProgram& getShader() const { return m_shader; }
	private:
		graphics::ShaderProgram m_shader;
		graphics::Model m_model;
		float m_time;
	};

}