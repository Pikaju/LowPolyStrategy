#include "EntityManager.h"

#include "Entity.h"

#include "structure/Tree.h"
#include "../../util/Noise.h"

namespace world {

	EntityManager::EntityManager(World* world) : m_world(world), m_entities(), m_shader()
	{
	}

	EntityManager::~EntityManager()
	{
	}

	void EntityManager::initialize()
	{
		m_shader.initialize();
		m_shader.loadShader("res/shaders/Entity.vert", graphics::SHADERTYPE_VERTEX);
		m_shader.loadShader("res/shaders/Entity.frag", graphics::SHADERTYPE_FRAGMENT);
		m_shader.link();

		util::Noise noise(0);
		for (int x = 0; x < 512; x++) {
			for (int y = 0; y < 512; y++) {
				if (rand() % 2 == 0) {
					continue;
				}
				float h = m_world->getChunkManager().getHeight(x, y);
				if (h < 2.0f || h > 4.0f) {
					continue;
				}
				if (noise.noise(x / 8.0f, 0.0f, y / 8.0f) < 0.2f) {
					continue;
				}
				Tree* tree = new Tree();
				addEntity(tree, getUnusedID());
				tree->setPosition(x, y);
			}
		}
	}

	void EntityManager::cleanup()
	{
		for (unsigned int i = 0; i < m_entities.size(); i++) {
			if (m_entities[i] == nullptr) {
				continue;
			}
			delete m_entities[i];
		}
		m_entities.clear();
		
		m_shader.cleanup();
	}

	void EntityManager::update(float delta)
	{
		for (unsigned int i = 0; i < m_entities.size(); i++) {
			if (m_entities[i] == nullptr) {
				continue;
			}
			m_entities[i]->update(delta);
			if (m_entities[i]->isRemoved()) {
				removeEntity(m_entities[i]);
				i--;
			}
		}
	}
	
	void EntityManager::render(const math::Mat4f& viewMatrix, const math::Mat4f& projectionMatrix)
	{
		m_shader.enable();
		m_shader.setUniform("fowTexture", 0);
		m_shader.setUniform("viewMatrix", viewMatrix);
		m_shader.setUniform("projectionMatrix", projectionMatrix);
		
		for (unsigned int i = 0; i < m_entities.size(); i++) {
			if (m_entities[i] == nullptr) {
				continue;
			}
			m_entities[i]->render(m_shader);
		}

		m_shader.disable();
	}

	void EntityManager::addEntity(Entity* entity, unsigned int id)
	{
		entity->setWorld(m_world);
		entity->setID(id);
		if (id >= m_entities.size()) {
			m_entities.resize((id + 1) * 2);
		}
		m_entities[id] = entity;
	}

	Entity* EntityManager::getEntity(unsigned int id)
	{
		return m_entities[id];
	}

	void EntityManager::removeEntity(Entity* entity)
	{
		m_entities[entity->getID()] = nullptr;
		delete entity;
	}

	unsigned int EntityManager::getUnusedID() const
	{
		unsigned int id = 0;
		while (id < m_entities.size() && m_entities[id] != nullptr) id++;
		return id;
	}

}