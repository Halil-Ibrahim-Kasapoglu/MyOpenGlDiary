//
//  Shader.hpp
//  Momentum
//
//  Created by Halil Ibrahim Kasapoglu on 17.05.2021.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>
#include <string>

/*
 * There is a lot work to do in uniforms
 */

class Shader {

private:
    unsigned int m_Rendered_ID;
public:
    Shader(const std::string& vertexShaderFilepath , const std::string& fragmentShaderFilepath);
    ~Shader();
    
    void Bind() const;
    void Unbind() const;
    
    void SetUniform4f(const std::string& name , float v0 , float v1, float v2, float v3);
    void SetUniformMatrix4fv(const std::string& name , unsigned int count , bool transpose , const float * value);

private:
    
    std::string GetShaderSource(std::string filePath);
    int GetUniformLocation(const std::string& name);
    unsigned int CreateShader(const std::string& vertexShader , const std::string& fragmentShader);
    unsigned int CompileShader(const std::string &shaderSource , unsigned int shaderType);
};

#endif /* Shader_hpp */
