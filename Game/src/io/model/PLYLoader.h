#pragma once

#include "../../graphics/model/Mesh.h"
#include "../../math/Vec3.h"

namespace io {

	class PLYVertex
	{
	public:
		math::Vec3f position;
		math::Vec3f normal;
		math::Vec3b color;

		static void getAttributes(std::vector<graphics::Attribute>& attributes)
		{
			attributes.emplace_back(graphics::TYPE_FLOAT, 3);
			attributes.emplace_back(graphics::TYPE_FLOAT, 3);
			attributes.emplace_back(graphics::TYPE_UNSIGNED_BYTE, 4, true);
		}
	};

	extern graphics::Model loadPLY(const char* file);
	extern void loadPLY(const char* file, graphics::Mesh<PLYVertex, unsigned int>& mesh);

}