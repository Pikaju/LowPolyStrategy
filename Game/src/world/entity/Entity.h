#pragma once

#include "../../graphics/shader/ShaderProgram.h"
#include "../World.h"

namespace world {

	class Entity
	{
	public:
		Entity();
		~Entity();

		virtual void update(float delta) {  }
		virtual void clientUpdate(float delta) {  }
		virtual void render(const graphics::ShaderProgram& shader) {  }

		inline World* getWorld() { return m_world; }
		inline void setWorld(World* world) { m_world = world; }

		inline void setID(unsigned int id) { m_id = id; }
		inline unsigned int getID() const { return m_id; }

		inline bool isRemoved() const { return m_removed; }
		inline void remove() { m_removed = true; }
	private:
		World* m_world;
		unsigned int m_id;
		bool m_removed;
	};

}