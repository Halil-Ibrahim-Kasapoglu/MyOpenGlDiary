//
//  Rigidbody.hpp
//  Momentum
//
//  Created by Halil Ibrahim Kasapoglu on 19.05.2021.
//

#ifndef Rigidbody_hpp
#define Rigidbody_hpp

#include <stdio.h>
#include "Vector.hpp"

class Rigidbody {
protected:
    Vec2 velocity;
    float mass;
    Vec2 momentum;
public:
    void SetVelocity(Vec2 v);
    Vec2 GetVelocity();
    void SetMass(float m);
    float GetMass();
    Vec2 GetMomentum();
};

#endif /* Rigidbody_hpp */
