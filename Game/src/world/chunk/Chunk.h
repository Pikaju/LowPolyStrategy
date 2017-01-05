#pragma once

#include "../../math/Vec2.h"
#include "../../math/Vec3.h"
#include "tile/Heightmap.h"
#include "../../graphics/model/Mesh.h"
#include "../../graphics/texture/Texture.h"

#define CHUNK_SIZE 32

namespace world {

	class ChunkManager;

	class Vertex
	{
	public:
		math::Vec3f position;
		math::Vec3f normal;
		math::Vec3b color;

		Vertex() : position(), normal(), color() {  }
		Vertex(const math::Vec3f& position, const math::Vec3f& normal = math::Vec3f(), const math::Vec3b& color = math::Vec3b()) : position(position), normal(normal), color(color) {  }
		~Vertex() {  }
	};

	enum NeighborDirection
	{
		NEIGHBORDIRECTION_RIGHT,
		NEIGHBORDIRECTION_LEFT,
		NEIGHBORDIRECTION_FRONT,
		NEIGHBORDIRECTION_BACK
	};

	class Chunk
	{
	public:
		Chunk(ChunkManager* chunkManager, const math::Vec2i& position);
		~Chunk();

		void initialize();
		void cleanup();

		void update(float delta);
		void render();

		void createMesh();
		void createFOWTexture();

		inline Tile& getTile(unsigned int x, unsigned int y) { return m_heightmap.getTile(x, y); }
		Tile& getTileRelative(int x, int y);

		inline const graphics::Texture& getFOWTexture() const { return m_fow; }
		 
		inline const math::Vec2i& getPosition() const { return m_position; }
		inline void setNeighbor(const NeighborDirection& direction, Chunk* neighbor) { m_numNeighbors += neighbor == nullptr ? (m_neighbors[direction] == nullptr ? 0 : -1) : 1; m_neighbors[direction] = neighbor; }
		inline bool isModelCreated() const { return m_modelCreated; }
	private:
		ChunkManager* m_chunkManager;
		math::Vec2i m_position;
		
		Heightmap<CHUNK_SIZE> m_heightmap;

		graphics::Mesh<Vertex, unsigned int> m_mesh;
		graphics::Model m_model;
		bool m_modelCreated;

		graphics::Texture m_fow;

		Chunk* m_neighbors[4];
		unsigned int m_numNeighbors;
	};

}