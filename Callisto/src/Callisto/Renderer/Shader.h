#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Callisto
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual const std::string& GetName() const = 0;
		virtual void SetName(const std::string& name) = 0;
	public:
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		static Ref<Shader> Create(const std::string& path);
		
	protected:
		static constexpr size_t MAX_SHADER_COUNT = 4;
	};

	class ShaderLibrary
	{
	public:
		void Add(Ref<Shader> shader);
		void Add(const std::string& name, Ref<Shader> shader);
		Ref<Shader> Get(const std::string& name);
		Ref<Shader> Load(const std::string& filePath);
		Ref<Shader> Load(const std::string& name, const std::string& filePath);
		bool Exist(const std::string& name);
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}