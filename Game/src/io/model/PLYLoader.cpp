#include "PLYLoader.h"

#include <fstream>
#include <sstream>

#include "../../debug/Debug.h"
#include "../../graphics/model/Mesh.h"

namespace io {

	graphics::Model loadPLY(const char* file)
	{
		graphics::Mesh<PLYVertex, unsigned int> mesh;
		PLYVertex::getAttributes(mesh.getAttributes());
		loadPLY(file, mesh);

		graphics::Model model;
		model.initialize();
		mesh.createModel(model);
		return model;
	}

	void loadPLY(const char* file, graphics::Mesh<PLYVertex, unsigned int>& mesh)
	{
		std::ifstream in(file);
		if (!in.is_open()) {
			debug::crash("Could not load file: \"" + std::string(file) + "\"");
		}

		std::string line;
		while (std::getline(in, line)) {
			PLYVertex vertex;
			int r;
			int g;
			int b;
			unsigned int numFaces;
			unsigned int v0;
			unsigned int v1;
			unsigned int v2;

			std::stringstream ss(line);
			if (ss >> vertex.position.x >> vertex.position.y >> vertex.position.z >> vertex.normal.x >> vertex.normal.y >> vertex.normal.z >> r >> g >> b) {
				vertex.color.x = (unsigned char)r;
				vertex.color.y = (unsigned char)g;
				vertex.color.z = (unsigned char)b;
				mesh.getVertices().push_back(vertex);
			}
			else if ((ss = std::stringstream(line)) >> numFaces >> v0 >> v1 >> v2) {
				mesh.getIndices().push_back(v0);
				mesh.getIndices().push_back(v1);
				mesh.getIndices().push_back(v2);
				if (numFaces == 4) {
					unsigned int v3;
					ss >> v3;
					mesh.getIndices().push_back(v0);
					mesh.getIndices().push_back(v2);
					mesh.getIndices().push_back(v3);
				}
			}
		}
	}

}