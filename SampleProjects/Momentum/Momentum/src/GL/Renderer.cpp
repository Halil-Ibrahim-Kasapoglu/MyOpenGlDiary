//
//  Renderer.cpp
//  Momentum
//
//  Created by Halil Ibrahim Kasapoglu on 17.05.2021.
//

#include "Renderer.hpp"
#include <iostream>

void Renderer::Clear(){
    glClearColor(0, 0, 0, 1);
    glClear( GL_COLOR_BUFFER_BIT );
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const{
    shader.Bind();
    va.Bind();
    ib.Bind();
    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}
