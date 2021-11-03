//
//  VertexArray.hpp
//  Momentum
//
//  Created by Halil Ibrahim Kasapoglu on 17.05.2021.
//

#ifndef VertexArray_hpp
#define VertexArray_hpp

#include <stdio.h>

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray {

private:
    
    unsigned int m_RendererID;
public:
    VertexArray();
    ~VertexArray();
    
    void AddBuffer(const VertexBuffer& buffer , const VertexBufferLayout& layout);
    void Bind() const;
    void Unbind() const;

};


#endif /* VertexArray_hpp */
