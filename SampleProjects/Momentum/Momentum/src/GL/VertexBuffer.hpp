//
//  VertexBuffer.hpp
//  Momentum
//
//  Created by Halil Ibrahim Kasapoglu on 17.05.2021.
//

#ifndef VertexBuffer_hpp
#define VertexBuffer_hpp

#include <stdio.h>

class VertexBuffer{
    
private:
    unsigned int m_RendererID;
    
public:
    VertexBuffer(const void * data , unsigned int size);
    ~VertexBuffer();
    
    void Bind() const;
    void Unbind() const;
    
};


#endif /* VertexBuffer_hpp */
