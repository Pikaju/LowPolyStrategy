#pragma once

#include <vector>

#include "../../math/Mat4.h"
#include "../../graphics/shader/ShaderProgram.h"

namespace world {

	class World;
	class Entity;

	class EntityManager
	{
	public:
		EntityManager(World* world);
		~EntityManager();

		void initialize();
		void cleanup();

		void update(float delta);
		void render(const math::Mat4f& viewMatrix, const math::Mat4f& projectionMatrix);

		void addEntity(Entity* entity, unsigned int id);
		Entity* getEntity(unsigned int id);
		void removeEntity(Entity* entity);
		unsigned int getUnusedID() const;
	private:
		World* m_world;

		std::vector<Entity*> m_entities;

		graphics::ShaderProgram m_shader;
	};

}