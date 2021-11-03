//
//  Circle.hpp
//  Momentum
//
//  Created by Halil Ibrahim Kasapoglu on 17.05.2021.
//

#ifndef Circle_hpp
#define Circle_hpp

#include <stdio.h>
#include "Vector.hpp"
#include "Rigidbody.hpp"

#define DEFAULT_CIRCLE_SMOOTHNESS (40)

class Circle : public Rigidbody {
    
private:
    float * colorData;
    float radius;
    Vec2 origin;

public:
    Circle();
    Circle(Vec2 o, float r);
    float Area();
    
    float GetRadius();
    void SetRadius(float r);
    
    Vec2 GetOrigin();
    void SetOrigin(Vec2 o);
    
    void SetColorData(float * cd);
    float * GetColorData();
    
    unsigned int * indexData(const unsigned int vertexCnt);
    unsigned int * indexData();
    float * vertexData(const unsigned int vertexCnt);
    float * vertexData();
    
    static unsigned int * CircleIndexData();
    static float * CircleVertexData();
    
    void Move(float timeScale);
    bool CollideWith(Circle o);
    bool CollideWithPoint(Vec2 p);

    static void ApplyElasticCollision(Circle &c1, Circle &c2);
    
};


#endif /* Circle_hpp */
