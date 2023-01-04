#include "CallistoPCH.h"
#include "Shader.h"

#include <Callisto/Renderer/RendererAPI.h>
#include <Callisto/Platforms/OpenGL/OpenGLShader.h>

namespace Callisto
{
	/******************************************************************************************************************************/
	/* Shader */
	/******************************************************************************************************************************/
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::NONE:
			CALLISTO_CORE_ASSERT(false, "RenderAPI::NONE not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}
		CALLISTO_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& path)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::NONE:
			CALLISTO_CORE_ASSERT(false, "RenderAPI::NONE not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(path);
		}
		CALLISTO_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
	void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& float4)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::NONE:
			CALLISTO_CORE_ASSERT(false, "RenderAPI::NONE not supported!");
		case RendererAPI::API::OpenGL:
			dynamic_cast<OpenGLShader*>(this)->UploadUniformFloat4(name, float4);
		}
		CALLISTO_CORE_ASSERT(false, "Unknown RendererAPI");
	}
	void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::NONE:
			CALLISTO_CORE_ASSERT(false, "RenderAPI::NONE not supported!");
		case RendererAPI::API::OpenGL:
			dynamic_cast<OpenGLShader*>(this)->UploadUniformMat4(name, matrix);
		}
		CALLISTO_CORE_ASSERT(false, "Unknown RendererAPI");
	}
	void Shader::UploadUniformInt(const std::string& name, int value)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::NONE:
			CALLISTO_CORE_ASSERT(false, "RenderAPI::NONE not supported!");
		case RendererAPI::API::OpenGL:
			dynamic_cast<OpenGLShader*>(this)->UploadUniformInt(name, value);
		}
		CALLISTO_CORE_ASSERT(false, "Unknown RendererAPI");
	}
	/******************************************************************************************************************************/
	/* ShaderLibrary */
	/******************************************************************************************************************************/

	void ShaderLibrary::Add(Ref<Shader> shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}
	void ShaderLibrary::Add(const std::string& name, Ref<Shader> shader)
	{
		CALLISTO_CORE_ASSERT(!Exist(name), "Shader already exist in library!");
		shader->SetName(name);
		m_Shaders[name] = shader;
	}


	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		CALLISTO_CORE_ASSERT(Exist(name), "Shader doesnt exist in library!");
		return m_Shaders[name];
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		Ref<Shader> shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath)
	{
		Ref<Shader> shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	bool ShaderLibrary::Exist(const std::string& name)
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}