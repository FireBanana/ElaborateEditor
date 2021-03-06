#include "Shader.h"
#include "Logger.h"

#include <string>

Shader::Shader(const char* vertexSource, const char* fragmentSource)
{
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShaderFile.open(vertexSource);
        fShaderFile.open(fragmentSource);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        m_VertexCode = vShaderStream.str();
        m_FragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        LOG_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
        return;
    }

    CreateShaders();
    CreateProgram();
    DeleteShaders();
    Use();
}

void Shader::DrawIndexed(const Buffer<float>& vertexBuffer, const Buffer<int>& elementBuffer)
{
    elementBuffer.Bind();
    vertexBuffer.Bind();
    
    glDrawElements(
        GL_TRIANGLES,
        elementBuffer.GetSize(),
        GL_UNSIGNED_INT,
        0
    );
}

void Shader::Draw(const Buffer<float>& vertexBuffer, int count)
{
    vertexBuffer.Bind();

    glDrawArrays(GL_TRIANGLES, 0, count);
}

void Shader::SetUniformFloat4(const char* name, const glm::mat4& matrix)
{
    m_UniformList[name] = matrix;

    int uniformLoc = glGetUniformLocation(m_Id, name);
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(m_UniformList[name]));
}

void Shader::ResetSources(std::string& vertexSource, std::string& fragmentSource)
{
    m_VertexCode = vertexSource;
    m_FragmentCode = fragmentSource;

    CreateShaders();
    CreateProgram();
    DeleteShaders();
    Use();

    for (auto& item : m_UniformList)
        SetUniformFloat4(item.first.c_str(), item.second);
}

void Shader::CreateShaders()
{
    const char* vc = m_VertexCode.c_str();
    const char* fc = m_FragmentCode.c_str();

    m_Vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_Vertex, 1, &vc, nullptr);
    glCompileShader(m_Vertex);
    CheckErrors(m_Vertex, "VERTEX");

    m_Fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_Fragment, 1, &fc, nullptr);
    glCompileShader(m_Fragment);
    CheckErrors(m_Fragment, "FRAGMENT");
}

void Shader::CreateProgram()
{
    m_Id = glCreateProgram();
    glAttachShader(m_Id, m_Vertex);
    glAttachShader(m_Id, m_Fragment);
    glLinkProgram(m_Id);
    CheckErrors(m_Id, "PROGRAM");
}

void Shader::DeleteShaders()
{
    glDeleteShader(m_Vertex);
    glDeleteShader(m_Fragment);
}

void Shader::CheckErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            LOG_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: " + type);
            LOG_ERROR(infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            LOG_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: " + type);
            LOG_ERROR(infoLog);
        }
    }
}
