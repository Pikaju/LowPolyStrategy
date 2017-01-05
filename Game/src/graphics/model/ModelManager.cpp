#include "ModelManager.h"

#include "../../io/model/PLYLoader.h"

namespace graphics {

	std::unordered_map<std::string, Model> ModelManager::m_models;
	std::vector<Model*> ModelManager::m_modelVector;

	Model* ModelManager::getModel(const std::string& file)
	{
		auto& it = m_models.find(file);
		if (it != m_models.end()) return &it->second;
		m_models[file] = io::loadPLY(file.c_str());
		m_modelVector.push_back(&m_models[file]);
		return m_modelVector.back();
	}

	void ModelManager::cleanup()
	{
		for (unsigned int i = 0; i < m_modelVector.size(); i++) {
			m_modelVector[i]->cleanup();
		}
	}

}