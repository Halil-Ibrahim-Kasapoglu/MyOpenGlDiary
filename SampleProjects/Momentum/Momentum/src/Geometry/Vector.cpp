//
//  Vector.cpp
//  Momentum
//
//  Created by Halil Ibrahim Kasapoglu on 17.05.2021.
//

#include "Vector.hpp"
#include <math.h>

Vec2::Vec2(){
    x = y = 0;
}
Vec2::Vec2(float _x, float _y){
    x = _x;
    y = _y;
}
float Vec2::lenght(){
    return sqrt(x * x + y * y);
}
Vec2 Vec2::unit(){
    return Vec2(x / lenght() , y / lenght());
}
Vec2 Vec2::operator*(float c){
    return Vec2(x * c , y * c);
}
Vec2 Vec2::operator+(Vec2 v){
    return Vec2(x + v.getX() , y + v.getY());
}
Vec2 Vec2::operator-(Vec2 v){
    return Vec2(x-v.getX() , y-v.getY());
}
Vec2 Vec2::operator-(){
    return Vec2(-x,-y);
}
std::string Vec2::tostr(){
    return "("+std::to_string(x) + ", " + std::to_string(y) + ")";
}
float Vec2::getX(){
    return x;
}
float Vec2::getY(){
    return y;
}
float Vec2::dotProduct(Vec2 a, Vec2 b){
    return a.getX() * b.getX() + a.getY() * b.getY();
}
float Vec2::crossProductMag(Vec2 a, Vec2 b){
    return a.getX() * b.getY() - b.getX()*a.getY();
}
Vec2 Vec2::projOn(Vec2 u){
    return u * (dotProduct(Vec2(x,y), u) / u.lenght() / u.lenght());
}

