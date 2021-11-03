//
//  Rigidbody.cpp
//  Momentum
//
//  Created by Halil Ibrahim Kasapoglu on 19.05.2021.
//

#include "Rigidbody.hpp"

float Rigidbody::GetMass(){
    return mass;
}
void Rigidbody::SetMass(float m){
    mass = m;
}

Vec2 Rigidbody::GetMomentum(){
    return velocity * mass;
}

Vec2 Rigidbody::GetVelocity(){
    return velocity;
}
void Rigidbody::SetVelocity(Vec2 v){
    velocity = v;
}
