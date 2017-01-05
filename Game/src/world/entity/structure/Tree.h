#pragma once

#include "Structure.h"

namespace graphics {
	class Model;
}

namespace world {

	class Tree : public Structure
	{
	public:
		Tree();
		~Tree();

		void render(const graphics::ShaderProgram& shader) override;
	private:
		graphics::Model* m_model;
		float m_rotation;
	};

}