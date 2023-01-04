#pragma once

#include <Callisto/Renderer/Shader.h>

//REMOVE THIS
typedef unsigned int GLenum;

namespace Callisto
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertSrc, const std::string& fragSrc);
		OpenGLShader(const std::string& path);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const override { return m_Name; }
		virtual void SetName(const std::string& name) override { m_Name = name; }


		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		
		void UploadUniformInt(const std::string& name, int value);
		
		void UploadUniformFloat1(const std::string& name, float float1);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& float2);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& float3);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& float4);
	private:
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& shaderSrc);
	private:
		uint32_t m_RendererID{ 0 };
		std::string m_Name;
	};
}