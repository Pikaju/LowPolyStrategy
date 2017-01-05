#include "ChunkManager.h"

#include "Chunk.h"
#include "../../graphics/shader/ShaderProgram.h"
#include "../../math/Mat4.h"

namespace world {

	ChunkManager::ChunkManager(World* world) : m_world(world), m_chunkVector(), m_chunkMap(), m_water()
	{
	}
	
	ChunkManager::~ChunkManager()
	{
	}

	void ChunkManager::initialize()
	{
		m_shader.initialize();
		m_shader.loadShader("res/shaders/Chunk.vert", graphics::SHADERTYPE_VERTEX);
		m_shader.loadShader("res/shaders/Chunk.frag", graphics::SHADERTYPE_FRAGMENT);
		m_shader.link();

		m_water.initialize(CHUNK_SIZE, 1.0f);

		for (int x = -1; x < 17; x++) {
			for (int y = -1; y < 17; y++) {
				addChunk(new Chunk(this, math::Vec2i(x, y)));
				m_chunkVector.back()->initialize();
			}
		}
		for (int x = 0; x < 16; x++) {
			for (int y = 0; y < 16; y++) {
				getChunk(math::Vec2i(x, y))->createMesh();
			}
		}
	}

	void ChunkManager::cleanup()
	{
		for (unsigned int i = 0; i < m_chunkVector.size(); i++) {
			m_chunkVector[i]->cleanup();
			delete m_chunkVector[i];
		}

		m_water.cleanup();
		m_shader.cleanup();
	}

	void ChunkManager::update(float delta)
	{
		for (unsigned int i = 0; i < m_chunkVector.size(); i++) {
			m_chunkVector[i]->update(delta);
		}
		m_water.update(delta);
	}
	
	void ChunkManager::render(const math::Mat4f& viewMatrix, const math::Mat4f& projectionMatrix)
	{
		m_shader.enable();
		m_shader.setUniform("fowTexture", 0);
		m_shader.setUniform("viewMatrix", viewMatrix);
		m_shader.setUniform("projectionMatrix", projectionMatrix);
		m_shader.disable();

		for (unsigned int i = 0; i < m_chunkVector.size(); i++) {
			if (!m_chunkVector[i]->isModelCreated()) continue;
			math::Mat4f translationMatrix = math::Mat4f::translation((float)(m_chunkVector[i]->getPosition().x * CHUNK_SIZE), 0, (float)(m_chunkVector[i]->getPosition().y * CHUNK_SIZE));
			m_chunkVector[i]->getFOWTexture().use(0);
			m_shader.enable();
			m_shader.setUniform("modelMatrix", translationMatrix);
			m_shader.setUniform("chunkPosition", m_chunkVector[i]->getPosition() * (float)CHUNK_SIZE);
			m_chunkVector[i]->render();
			m_shader.disable();
		}
	}

	void ChunkManager::renderWater(const math::Mat4f& viewMatrix, const math::Mat4f& projectionMatrix, const math::Vec3f& cameraPosition)
	{
		m_water.getShader().enable();
		m_water.updateUniforms();
		m_water.getShader().setUniform("viewMatrix", viewMatrix);
		m_water.getShader().setUniform("projectionMatrix", projectionMatrix);
		m_water.getShader().setUniform("cameraPosition", cameraPosition);
		m_water.getShader().disable();

		for (unsigned int i = 0; i < m_chunkVector.size(); i++) {
			if (!m_chunkVector[i]->isModelCreated()) continue;
			m_chunkVector[i]->getFOWTexture().use(0);
			math::Mat4f translationMatrix = math::Mat4f::translation((float)(m_chunkVector[i]->getPosition().x * CHUNK_SIZE), 0, (float)(m_chunkVector[i]->getPosition().y * CHUNK_SIZE));
			m_water.getShader().enable();
			m_water.getShader().setUniform("modelMatrix", translationMatrix);
			m_water.getShader().setUniform("chunkPosition", m_chunkVector[i]->getPosition() * (float)CHUNK_SIZE);
			m_water.render();
			m_water.getShader().disable();
		}
	}

	void ChunkManager::addChunk(Chunk* chunk)
	{
		m_chunkVector.push_back(chunk);
		m_chunkMap[chunk->getPosition()] = chunk;
	}

	Chunk* ChunkManager::getChunk(const math::Vec2i& position)
	{
		auto& it = m_chunkMap.find(position);
		if (it == m_chunkMap.end()) return nullptr;
		return it->second;
	}

	float ChunkManager::getHeight(int x, int y)
	{
		Chunk* chunk = getChunk(math::Vec2i(x / CHUNK_SIZE, y / CHUNK_SIZE));
		if (chunk == nullptr) return -1.0f;
		return chunk->getTile(x % CHUNK_SIZE, y % CHUNK_SIZE).getHeight();
	}

	float ChunkManager::getHeight(float x, float y)
	{
		float h00 = getHeight((int)x, (int)y);
		float h10 = getHeight((int)x + 1, (int)y);
		float h11 = getHeight((int)x + 1, (int)y + 1);
		float h01 = getHeight((int)x, (int)y + 1);
		float xo = x - floor(x);
		float yo = y - floor(y);

		float hx0 = (1.0f - xo) * h00 + xo * h10;
		float hx1 = (1.0f - xo) * h01 + xo * h11;
		return (1.0f - yo) * hx0 + yo * hx1;
	}

	int ChunkManager::getFOW(int x, int y)
	{
		Chunk* chunk = getChunk(math::Vec2i(x / CHUNK_SIZE, y / CHUNK_SIZE));
		if (chunk == nullptr) return -1;
		return chunk->getTile(x % CHUNK_SIZE, y % CHUNK_SIZE).getFOW();
	}

}