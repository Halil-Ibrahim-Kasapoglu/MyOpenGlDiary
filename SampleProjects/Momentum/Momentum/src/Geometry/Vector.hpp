//
//  Vector.hpp
//  Momentum
//
//  Created by Halil Ibrahim Kasapoglu on 17.05.2021.
//

#ifndef Vector_hpp
#define Vector_hpp

#include <stdio.h>
#include <iostream>

class Vec2 {
    float x;
    float y;
public:

    
    Vec2();
    Vec2(float _x , float _y);
    float lenght();
    float getX();
    float getY();
    Vec2 unit();
    
    Vec2 operator *(float c);
    Vec2 operator +(Vec2 v);
    Vec2 operator -(Vec2 v);
    Vec2 operator -();
    std::string tostr();
    
    static float dotProduct(Vec2 a , Vec2 b);
    static float crossProductMag(Vec2 a , Vec2 b);
    Vec2 projOn(Vec2 u);

};

#endif /* Vector_hpp */
