#pragma once

#include <Callisto/Renderer/Shader.h>

namespace Callisto
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertSrc, const std::string& fragSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		void UploadUniformInt(const std::string& name, int value);

		void UploadUniformFloat1(const std::string& name, float float1);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& float2);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& float3);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& float4);

	private:
		uint32_t m_RendererID;
	};
}