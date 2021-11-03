//
//  VertexArray.cpp
//  Momentum
//
//  Created by Halil Ibrahim Kasapoglu on 17.05.2021.
//

#include "VertexArray.hpp"
#include "Renderer.hpp"

VertexArray::VertexArray(){
    
    glGenVertexArrays(1 , &m_RendererID);
    
}

VertexArray::~VertexArray(){
    glDeleteVertexArrays(1 , &m_RendererID);
}

void VertexArray::Bind() const {
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer &buffer, const VertexBufferLayout &layout){
 
    Bind();
    buffer.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (int i = 0; i < elements.size(); i ++){
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i , element.count , element.type , element.normalized , layout.GetStride() , (const void *)offset);
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}
