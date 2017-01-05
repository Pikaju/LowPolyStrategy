#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Model.h"

namespace graphics {

	class ModelManager
	{
	public:
		static Model* getModel(const std::string& file);
		static void cleanup();
	private:
		static std::unordered_map<std::string, Model> m_models;
		static std::vector<Model*> m_modelVector;
	};

}