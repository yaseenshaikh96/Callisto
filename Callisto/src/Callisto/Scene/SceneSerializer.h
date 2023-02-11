#pragma once

#include "Callisto.h"
#include "Callisto/Scene/Scene.h"

namespace Callisto
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);
		
		void Serialize(const std::string& path);
		void SerializeRuntime(const std::string& path);

		bool DeSerialize(const std::string& path);
		bool DeSerializeRuntime(const std::string& path);

	private:
		Ref<Scene> m_Scene;
	};
}