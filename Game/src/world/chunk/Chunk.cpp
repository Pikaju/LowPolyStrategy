#include "Chunk.h"

#include "ChunkManager.h"
#include "../../graphics/Attribute.h"
#include "../../util/Noise.h"
#include "../../math/Vec4.h"

#include "../../graphics/model/ModelManager.h"

namespace world {

	Chunk::Chunk(ChunkManager* chunkManager, const math::Vec2i& position) : m_chunkManager(chunkManager), m_position(position), m_heightmap(), m_mesh(), m_model(), m_fow(), m_modelCreated(false)
	{
		m_mesh.addAttribute(graphics::Attribute(graphics::TYPE_FLOAT, 3));
		m_mesh.addAttribute(graphics::Attribute(graphics::TYPE_FLOAT, 3));
		m_mesh.addAttribute(graphics::Attribute(graphics::TYPE_UNSIGNED_BYTE, 4, true));
	}

	Chunk::~Chunk()
	{
	}

	void Chunk::initialize()
	{
		m_model.initialize();

		util::Noise noise(0);
		for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
			for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
				int xx = (int)x + m_position.x * CHUNK_SIZE;
				int yy = (int)y + m_position.y * CHUNK_SIZE;

				m_heightmap.getTile(x, y).increaseFOW();
			
				float n0 = noise.noise((float)xx / 2.0f, 0.0f, (float)yy / 2.0f) * 0.5f;
				float n1 = noise.noise((float)xx / 16.0f, 0.0f, (float)yy / 16.0f) * 2.0f;
				float n2 = noise.noise((float)xx / 32.0f, 0.0f, (float)yy / 32.0f) * 8.0f;
				float n3 = noise.noise((float)xx / 64.0f, 0.0f, (float)yy / 64.0f) * 32.0f;
				char a = rand() % 16;
				float t = n0 + n1 + n2 + n3 + 4.0f;
				float h = t;
				
				if (h < -2.0f) h = -2.0f;
				if (t + 4.0f < h) h = t + 4.0f;
				if (h < -4.0f) h = -4.0f;
				if (t + 8.0f < h) h = t + 8.0f;
				if (h < -6.0f) h = -6.0f;
				if (t + 16.0f < h) h = t + 16.0f;
				if (h < -8.0f) h = -8.0f;
				
				if (h > 2.0f) h = 2.0f;
				if (t - 4.0f > h) h = t - 4.0f;
				if (h > 4.0f) h = 4.0f;
				if (t - 16.0f > h) h = t - 16.0f;
				if (h > 6.0f) h = 6.0f;

				m_heightmap.getTile(x, y).setHeight(h);
				m_heightmap.getTile(x, y).setColor(math::Vec3b(64 + a, (unsigned char)(64.0f + (m_heightmap.getTile(x, y).getHeight() / 12.0f + 0.5f) * 128.0f) + a, 64 + a));
				if (m_heightmap.getTile(x, y).getHeight() > 4.0f) {
					if (m_heightmap.getTile(x, y).getHeight() > 5.0f) {
						m_heightmap.getTile(x, y).setColor(math::Vec3b(230 + a, 230 + a, 230 + a));
					}
					else {
						m_heightmap.getTile(x, y).setColor(math::Vec3b(128 + a, 128 + a, 128 + a));
					}
				}
				if (m_heightmap.getTile(x, y).getHeight() < 1.5f && m_heightmap.getTile(x, y).getHeight() > -1.0f) {
					m_heightmap.getTile(x, y).setColor(math::Vec3b(200 + a, 180 + a, 130 + a));
				}
			}
		}

		Chunk* neighbor = nullptr;
		if ((neighbor = m_chunkManager->getChunk(m_position + math::Vec2i(1, 0)))) { setNeighbor(NEIGHBORDIRECTION_RIGHT, neighbor); neighbor->setNeighbor(NEIGHBORDIRECTION_LEFT, this); }
		if ((neighbor = m_chunkManager->getChunk(m_position + math::Vec2i(-1, 0)))) { setNeighbor(NEIGHBORDIRECTION_LEFT, neighbor); neighbor->setNeighbor(NEIGHBORDIRECTION_RIGHT, this); }
		if ((neighbor = m_chunkManager->getChunk(m_position + math::Vec2i(0, 1)))) { setNeighbor(NEIGHBORDIRECTION_FRONT, neighbor); neighbor->setNeighbor(NEIGHBORDIRECTION_BACK, this); }
		if ((neighbor = m_chunkManager->getChunk(m_position + math::Vec2i(0, -1)))) { setNeighbor(NEIGHBORDIRECTION_BACK, neighbor); neighbor->setNeighbor(NEIGHBORDIRECTION_FRONT, this); }
	}
	
	void Chunk::cleanup()
	{
		m_fow.cleanup();
		m_model.cleanup();

		if (m_neighbors[NEIGHBORDIRECTION_RIGHT] != nullptr) m_neighbors[NEIGHBORDIRECTION_RIGHT]->setNeighbor(NEIGHBORDIRECTION_LEFT, nullptr);
		if (m_neighbors[NEIGHBORDIRECTION_LEFT] != nullptr) m_neighbors[NEIGHBORDIRECTION_LEFT]->setNeighbor(NEIGHBORDIRECTION_RIGHT, nullptr);
		if (m_neighbors[NEIGHBORDIRECTION_FRONT] != nullptr) m_neighbors[NEIGHBORDIRECTION_FRONT]->setNeighbor(NEIGHBORDIRECTION_BACK, nullptr);
		if (m_neighbors[NEIGHBORDIRECTION_BACK] != nullptr) m_neighbors[NEIGHBORDIRECTION_BACK]->setNeighbor(NEIGHBORDIRECTION_FRONT, nullptr);
	}

	void Chunk::update(float delta)
	{
		if (!m_mesh.isEmpty()) {
			m_mesh.createModel(m_model);
			m_mesh.clear();
			m_modelCreated = true;
		}
		if (m_modelCreated) {
			//createFOWTexture();
		}
	}
	
	void Chunk::render()
	{
		m_model.render();
	}

	void Chunk::createMesh()
	{
		m_modelCreated = false;
		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int y = 0; y < CHUNK_SIZE; y++) {
				Tile& t0 = getTileRelative(x, y);
				Tile& t1 = getTileRelative(x, y + 1);
				Tile& t2 = getTileRelative(x + 1, y + 1);
				Tile& t3 = getTileRelative(x + 1, y);
				
				Vertex v0;
				Vertex v1;
				Vertex v2;
				Vertex v3;
				float ao0;
				float ao1;
				float ao2;
				float ao3;

				if ((x + y) % 2 == 0) {
					v0 = Vertex(math::Vec3f((float)x, t0.getHeight(), (float)y));
					v1 = Vertex(math::Vec3f((float)x, t1.getHeight(), (float)(y + 1)));
					v2 = Vertex(math::Vec3f((float)(x + 1), t2.getHeight(), (float)(y + 1)));
					v3 = Vertex(math::Vec3f((float)(x + 1), t3.getHeight(), (float)y));
					ao0 = 1.0f - ((getTileRelative(x - 1, y).getHeight() + getTileRelative(x + 1, y).getHeight() + getTileRelative(x, y - 1).getHeight() + getTileRelative(x, y + 1).getHeight()) / 4.0f - v0.position.y) * 0.75f;
					ao1 = 1.0f - ((getTileRelative(x - 1, y + 1).getHeight() + getTileRelative(x + 1, y + 1).getHeight() + getTileRelative(x, y).getHeight() + getTileRelative(x, y + 2).getHeight()) / 4.0f - v1.position.y) * 0.75f;
					ao2 = 1.0f - ((getTileRelative(x, y + 1).getHeight() + getTileRelative(x + 2, y + 1).getHeight() + getTileRelative(x + 1, y).getHeight() + getTileRelative(x + 1, y + 2).getHeight()) / 4.0f - v2.position.y) * 0.75f;
					ao3 = 1.0f - ((getTileRelative(x, y).getHeight() + getTileRelative(x + 2, y).getHeight() + getTileRelative(x + 1, y - 1).getHeight() + getTileRelative(x + 1, y + 1).getHeight()) / 4.0f - v3.position.y) * 0.75f;
				}
				else {
					v0 = Vertex(math::Vec3f((float)x, t1.getHeight(), (float)(y + 1)));
					v1 = Vertex(math::Vec3f((float)(x + 1), t2.getHeight(), (float)(y + 1)));
					v2 = Vertex(math::Vec3f((float)(x + 1), t3.getHeight(), (float)y));
					v3 = Vertex(math::Vec3f((float)x, t0.getHeight(), (float)y));
					ao0 = 1.0f - ((getTileRelative(x - 1, y + 1).getHeight() + getTileRelative(x + 1, y + 1).getHeight() + getTileRelative(x, y).getHeight() + getTileRelative(x, y + 2).getHeight()) / 4.0f - v0.position.y) * 0.75f;
					ao1 = 1.0f - ((getTileRelative(x, y + 1).getHeight() + getTileRelative(x + 2, y + 1).getHeight() + getTileRelative(x + 1, y).getHeight() + getTileRelative(x + 1, y + 2).getHeight()) / 4.0f - v1.position.y) * 0.75f;
					ao2 = 1.0f - ((getTileRelative(x, y).getHeight() + getTileRelative(x + 2, y).getHeight() + getTileRelative(x + 1, y - 1).getHeight() + getTileRelative(x + 1, y + 1).getHeight()) / 4.0f - v2.position.y) * 0.75f;
					ao3 = 1.0f - ((getTileRelative(x - 1, y).getHeight() + getTileRelative(x + 1, y).getHeight() + getTileRelative(x, y - 1).getHeight() + getTileRelative(x, y + 1).getHeight()) / 4.0f - v3.position.y) * 0.75f;
				}

				math::Vec3f n0 = (v1.position - v0.position).cross(v2.position - v0.position).normalized();
				math::Vec3f n1 = (v1.position - v3.position).cross(v2.position - v3.position).normalized();

				math::Vec3b c0 = (math::Vec3b)(((math::Vec3i)t0.getColor() + (math::Vec3i)t1.getColor() + (math::Vec3i)t2.getColor()) / 3);
				math::Vec3b c1 = (math::Vec3b)(((math::Vec3i)t0.getColor() + (math::Vec3i)t2.getColor() + (math::Vec3i)t3.getColor()) / 3);
				//math::Vec3b c0 = (math::Vec3b)((math::Vec3i)t2.getColor());
				//math::Vec3b c1 = (math::Vec3b)((math::Vec3i)t3.getColor());
				
				if (ao0 > 1.0f) ao0 = 1.0f;
				if (ao1 > 1.0f) ao1 = 1.0f;
				if (ao2 > 1.0f) ao2 = 1.0f;
				if (ao3 > 1.0f) ao3 = 1.0f;

				v0.normal = n0;
				v1.normal = n0;
				v2.normal = n0;
				v0.color = c0 * ao0;
				v1.color = c0 * ao1;
				v2.color = c0 * ao2;

				m_mesh.triangle(v0, v1, v2);

				v0.normal = n1;
				v2.normal = n1;
				v3.normal = n1;
				v0.color = c1 * ao0;
				v2.color = c1 * ao2;
				v3.color = c1 * ao3;

				m_mesh.triangle(v0, v2, v3);
			}
		}

		createFOWTexture();
	}

	void Chunk::createFOWTexture()
	{
		math::Vec4b pixels[(CHUNK_SIZE + 2) * (CHUNK_SIZE + 2)];
		for (int x = -1; x < CHUNK_SIZE + 1; x++) {
			for (int y = -1; y < CHUNK_SIZE + 1; y++) {
				int fow = getTileRelative(x, y).getFOW();
				fow += getTileRelative(x + 1, y).getFOW() / 2;
				fow += getTileRelative(x - 1, y).getFOW() / 2;
				fow += getTileRelative(x, y + 1).getFOW() / 2;
				fow += getTileRelative(x, y - 1).getFOW() / 2;
				fow += getTileRelative(x + 1, y + 1).getFOW() / 4;
				fow += getTileRelative(x - 1, y + 1).getFOW() / 4;
				fow += getTileRelative(x + 1, y - 1).getFOW() / 4;
				fow += getTileRelative(x - 1, y - 1).getFOW() / 4;
				fow /= 4;
				pixels[(x + 1) + (y + 1) * (CHUNK_SIZE + 2)] = fow;
			}
		}
		m_fow.initialize(CHUNK_SIZE + 2, CHUNK_SIZE + 2, graphics::FORMAT_R8, pixels);
	}

	Tile& Chunk::getTileRelative(int x, int y)
	{
		if (x < 0) return m_neighbors[NEIGHBORDIRECTION_LEFT]->getTileRelative(x + CHUNK_SIZE, y);
		if (x >= CHUNK_SIZE) return m_neighbors[NEIGHBORDIRECTION_RIGHT]->getTileRelative(x - CHUNK_SIZE, y);
		if (y < 0) return m_neighbors[NEIGHBORDIRECTION_BACK]->getTileRelative(x, y + CHUNK_SIZE);
		if (y >= CHUNK_SIZE) return m_neighbors[NEIGHBORDIRECTION_FRONT]->getTileRelative(x, y - CHUNK_SIZE);
		return getTile((unsigned int)x, (unsigned int)y);
	}

}