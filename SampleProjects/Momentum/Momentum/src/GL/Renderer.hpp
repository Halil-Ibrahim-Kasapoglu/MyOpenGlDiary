//
//  Renderer.hpp
//  Momentum
//
//  Created by Halil Ibrahim Kasapoglu on 17.05.2021.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <stdio.h>
#include <GL/glew.h>

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

class Renderer {

private:
    
public:
    
    void Clear();
    void Draw(const VertexArray& va ,const IndexBuffer& ib, const Shader& shader) const;
    
};


#endif /* Renderer_hpp */
