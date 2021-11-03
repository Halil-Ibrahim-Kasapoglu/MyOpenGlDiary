//
//  IndexBuffer.hpp
//  Momentum
//
//  Created by Halil Ibrahim Kasapoglu on 17.05.2021.
//

#ifndef IndexBuffer_hpp
#define IndexBuffer_hpp

#include <stdio.h>

class IndexBuffer{
    
private:
    unsigned int m_RendererID;
    unsigned int m_Count;
    
public:
    IndexBuffer(const unsigned int*data , unsigned int count);
    ~IndexBuffer();
    
    void Bind() const;
    void Unbind() const;
    
    inline unsigned int GetCount() const {return m_Count;}
    
};

#endif /* IndexBuffer_hpp */
