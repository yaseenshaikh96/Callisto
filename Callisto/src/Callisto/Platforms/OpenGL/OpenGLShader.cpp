#include "CallistoPCH.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>

namespace Callisto
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		CALLISTO_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& path)
	{
		std::string shaderSrc = ReadFile(path);
		auto shaderSources = PreProcess(shaderSrc);
		Compile(shaderSources);
	}

	std::string OpenGLShader::ReadFile(const std::string& path)
	{
		std::string result;
		std::ifstream in(path, std::ios::in, std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			CALLISTO_CORE_ERROR("Could not open file at: {0}", path);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& shaderSrc)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = shaderSrc.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = shaderSrc.find_first_of("\r\n", pos);
			CALLISTO_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = shaderSrc.substr(begin, eol - begin);
			CALLISTO_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = shaderSrc.find_first_not_of("\r\n", eol);
			pos = shaderSrc.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = shaderSrc.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? shaderSrc.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}



	OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSource;
		sources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(sources);
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program  = glCreateProgram();
		std::vector<GLenum> glShaderIds(shaderSources.size());
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string source = kv.second;

			GLuint shader = glCreateShader(type);
			const GLchar* sourceCStr = (const GLchar*)source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				CALLISTO_CORE_ERROR("{0}", infoLog.data());
				CALLISTO_CORE_ASSERT(false, "Shader Compilation Error!");
				break;
			}
			glAttachShader(program, shader);
			glShaderIds.push_back(shader);
		}

		m_RendererID = program;
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			for (auto shader : glShaderIds)
			{
				glDeleteShader(shader);
			}

			CALLISTO_CORE_ERROR("{0}", infoLog.data());
			CALLISTO_CORE_ASSERT(false, "Shader Linking Error!");
			return;
		}
		for (auto shader : glShaderIds)
		{
			glDetachShader(program, shader);
		}
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}
	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}
	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}
	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));

	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}


	void OpenGLShader::UploadUniformFloat1(const std::string& name, float float1)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, float1);
	}
	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& float2)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, float2.x, float2.y);
	}
	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& float3)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, float3.x, float3.y, float3.z);
	}
	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& float4)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, float4.x, float4.y, float4.z, float4.w);
	}
}