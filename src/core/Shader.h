#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <unordered_map>

#include "Buffer.h"

class Shader
{
public:

	Shader(const char* vertexSource, const char* fragmentSource);

	inline void Use() { glUseProgram(m_Id); };

	void DrawIndexed(const Buffer<float>& vertexBuffer, const Buffer<int>& elementBuffer);
	void Draw(const Buffer<float>& vertexBuffer, int count);
	void SetUniformFloat4(const char* name, const glm::mat4& matrix);
	void ResetSources(std::string& vertexSource, std::string& fragmentSource);

private:

	std::unordered_map<std::string, glm::mat4> m_UniformList;

	unsigned int m_Id;
	unsigned int m_Vertex;
	unsigned int m_Fragment;

	std::string m_VertexCode;
	std::string m_FragmentCode;

	void CreateShaders();
	void CreateProgram();
	void DeleteShaders();
	void CheckErrors(unsigned int shader, std::string type);
};
