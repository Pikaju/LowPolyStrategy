#pragma once

#include "Tile.h"

namespace world {

	template<unsigned int size>
	class Heightmap
	{
	public:
		Heightmap() {  }
		~Heightmap() {  }

		inline Tile& getTile(unsigned int x, unsigned int y) { return m_tiles[x + y * size]; }
	private:
		Tile m_tiles[size * size];
	};

}