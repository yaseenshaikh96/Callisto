#include "CallistoPCH.h"
#include "SceneSerializer.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include "Callisto/Scene/Components.h"

namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

namespace Callisto
{

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}


	static void EntitySerializer(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << "12122329833";
		
		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag.c_str();

			out << YAML::EndMap; // TagComponent
		}
		
		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& tc = entity.GetComponent<TransformComponent>();

			out << YAML::Key << "Position" << YAML::Value << tc.Position;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap;
		}


		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cc = entity.GetComponent<CameraComponent>();
			auto& camera = cc.Camera;

			out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.FixedAspectRatio;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthoGraphicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthoGraphicFarClip();
			out << YAML::EndMap; // Camera


			out << YAML::EndMap; // CameraComponent
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent

			auto& src = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << src.Color;

			out << YAML::EndMap; // SpriteRendererComponent
		}


		out << YAML::EndMap; // Entity
	}

	/********************************************************************************************************************************************/
	/* SceneSerializer */
	/********************************************************************************************************************************************/
	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		:
		m_Scene(scene)
	{
	}

	void SceneSerializer::Serialize(const std::string& path)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		
		out << YAML::Key << "Scene" << YAML::Value << "SceneName";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto enttEntity)
		{
			Entity entity(enttEntity, m_Scene.get());
			if (!entity) return;
			
			EntitySerializer(out, entity);

		});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& path)
	{
		CALLISTO_ASSERT(false, "Not Implemented!");
	}
	
	bool SceneSerializer::DeSerialize(const std::string& path)
	{

		std::ifstream fileStream(path);
		std::stringstream stringStream;
		stringStream << fileStream.rdbuf();

		YAML::Node data = YAML::Load(stringStream.str());
		if (!data["Scene"]) return false;

		std::string sceneName = data["Scene"].as<std::string>();
		CALLISTO_CORE_INFO("Deseializing Scene: {0}", sceneName);

		YAML::Node entitiesNode = data["Entities"];
		if (entitiesNode)
		{
			for (YAML::Node entityNode : entitiesNode)
			{
				uint64_t uuid = entityNode["Entity"].as<uint64_t>();

				std::string name;
				YAML::Node tagComponentNode = entityNode["TagComponent"];
				if (tagComponentNode)
				{
					name = tagComponentNode["Tag"].as<std::string>();
				}

				Entity deserializedEntity=  m_Scene->CreateEntity(name);
				

				YAML::Node transformComponentNode = entityNode["TransformComponent"];
				if (transformComponentNode)
				{
					// Entities always have transforms
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Position = transformComponentNode["Position"].as<glm::vec3>();
					tc.Rotation = transformComponentNode["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponentNode["Scale"].as<glm::vec3>();
				}

				YAML::Node cameraComponentNode = entityNode["CameraComponent"];
				if (cameraComponentNode)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto cameraNode = cameraComponentNode["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraNode["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveVerticalFOV(cameraNode["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraNode["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraNode["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraNode["OrthographicSize"].as<float>());
					cc.Camera.SetOrthoGraphicNearClip(cameraNode["OrthographicNear"].as<float>());
					cc.Camera.SetOrthoGraphicFarClip(cameraNode["OrthographicFar"].as<float>());

					cc.Primary = cameraComponentNode["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponentNode["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponentNode = entityNode["SpriteRendererComponent"];
				if (spriteRendererComponentNode)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponentNode["Color"].as<glm::vec4>();
				}
			}
		}
		return true;
	}
	
	bool SceneSerializer::DeSerializeRuntime(const std::string& path)
	{
		CALLISTO_ASSERT(false, "Not Implemented!");
		return false;
	}
}