//
//  Shader.cpp
//  Momentum
//
//  Created by Halil Ibrahim Kasapoglu on 17.05.2021.
//

#include "Shader.hpp"
#include <fstream>
#include <iostream>
#include "Renderer.hpp"

Shader::Shader(const std::string& vertexShaderFilepath , const std::string& fragmentShaderFilepath)
:m_Rendered_ID(0) {

    std::string vertexShaderSource = GetShaderSource(vertexShaderFilepath);
    std::string fragmentShaderSource = GetShaderSource(fragmentShaderFilepath);
    m_Rendered_ID = CreateShader(vertexShaderSource, fragmentShaderSource);
    
    //    CompileShader();
    
}
Shader::~Shader(){
    
    glDeleteProgram(m_Rendered_ID);
}


void Shader::Bind() const{
    glUseProgram(m_Rendered_ID);
    
}
void Shader::Unbind() const{
    glUseProgram(0);
}

std::string Shader::GetShaderSource(std::string filePath){
    
    std::ifstream fileStream(filePath);
    std::string src = "";
    std::string line;
    while (getline(fileStream, line)){
        src += line + "\n";
    }
    fileStream.close();
    return src;
}

unsigned int Shader::CompileShader(const std::string &shaderSource , unsigned int shaderType){
    const char * ss = shaderSource.c_str();
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1 , &ss , NULL);
    glCompileShader(shader);
    
    std::cout<< ss << std::endl;
    
    int success;
    char infoLog[512];
    glGetShaderiv(shader , GL_COMPILE_STATUS , &success);
    if (!success){
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }
    return shader;
}

unsigned int Shader::CreateShader(const std::string& vertexShader , const std::string& fragmentShader){
    
    unsigned int program = glCreateProgram();

    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program , vs);
    glAttachShader(program , fs);
    glLinkProgram(program);
    glValidateProgram(program);
    
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3){
    glUniform4f(GetUniformLocation(name) , v0 , v1 , v2 ,v3);
}

void Shader::SetUniformMatrix4fv(const std::string &name, unsigned int count, bool transpose, const float *value){
    glUniformMatrix4fv(GetUniformLocation(name) , count, transpose , value);
}

int Shader::GetUniformLocation(const std::string &name){
    int location = glGetUniformLocation(m_Rendered_ID , name.c_str());
    return location;
}
