#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

class Shader
{
public:

	Shader(const char* vertexSource, const char* fragmentSource);

	inline void Use() { glUseProgram(m_Id); };

private:

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
