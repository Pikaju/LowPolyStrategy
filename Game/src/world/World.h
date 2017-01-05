#pragma once

#include "chunk/ChunkManager.h"
#include "entity/EntityManager.h"
#include "../graphics/shader/effect/SSAO.h"
#include "../graphics/shader/effect/Bloom.h"
#include "../graphics/texture/Framebuffer.h"
#include "../util/TPCamera.h"
#include "../util/FrustumCuller.h"

namespace graphics {
	class Window;
}

namespace io {
	class Input;
}

namespace world {

	class World
	{
	public:
		World();
		~World();

		void initialize();
		void cleanup();

		void update(const io::Input& input, float delta);
		void render(const graphics::Window& window);

		inline ChunkManager& getChunkManager() { return m_chunkManager; }
		inline const util::FrustumCuller& getFrustumCuller() const { return m_frustumCuller; }
	private:
		ChunkManager m_chunkManager;
		EntityManager m_entityManager;
		graphics::ShaderProgram m_screenShader;
		graphics::SSAO m_ssao;
		graphics::Bloom m_bloom;
		graphics::Framebuffer m_framebuffer;
		util::TPCamera m_camera;
		util::FrustumCuller m_frustumCuller;
	};

}