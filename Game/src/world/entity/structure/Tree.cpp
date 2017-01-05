#include "Tree.h"

#include "../../../graphics/model/ModelManager.h"
#include "../../../math/Mat4.h"

namespace world {

	Tree::Tree()
	{
		m_width = 1;
		m_height = 1;
		m_rotation = ((rand() % 128) / 128.0f) * 360.0f;
	}
	
	Tree::~Tree()
	{
	}

	void Tree::render(const graphics::ShaderProgram& shader)
	{
		float x = (float)m_x + (float)m_width / 2.0f;
		float y = getWorld()->getChunkManager().getHeight(m_x, m_y);
		float z = (float)m_y + (float)m_height / 2.0f;
		int fow = getWorld()->getChunkManager().getFOW(m_x, m_y);
		if (!getWorld()->getFrustumCuller().isSphereInFrustum(math::Vec3f(x, y + 0.5f, z), 1.5f)) return;
		shader.setUniform("modelMatrix", math::Mat4f::rotationY(m_rotation) * math::Mat4f::translation(x, y, z));
		shader.setUniform("fow", (float)fow / 255.0f);
		if (m_model == nullptr) {
			m_model = graphics::ModelManager::getModel(rand() % 2 == 0 ? "res/Tree.ply" : "res/Tree.ply");
		}
		m_model->render();
	}

}