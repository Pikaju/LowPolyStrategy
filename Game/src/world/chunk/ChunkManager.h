#pragma once

#include <vector>
#include <unordered_map>

#include "Water.h"
#include "../../math/Vec2.h"
#include "../../math/Vec3.h"
#include "../../math/Mat4.h"

namespace world {

	class World;
	class Chunk;

	class ChunkManager
	{
	public:
		ChunkManager(World* world);
		~ChunkManager();

		void initialize();
		void cleanup();

		void update(float delta);
		void render(const math::Mat4f& viewMatrix, const math::Mat4f& projectionMatrix);
		void renderWater(const math::Mat4f& viewMatrix, const math::Mat4f& projectionMatrix, const math::Vec3f& cameraPosition);

		void addChunk(Chunk* chunk);
		Chunk* getChunk(const math::Vec2i& position);

		float getHeight(int x, int y);
		float getHeight(float x, float y);
		int getFOW(int x, int y);
	private:
		World* m_world;

		std::vector<Chunk*> m_chunkVector;
		std::unordered_map<math::Vec2i, Chunk*> m_chunkMap;

		graphics::ShaderProgram m_shader;
		world::Water m_water;
	};

}