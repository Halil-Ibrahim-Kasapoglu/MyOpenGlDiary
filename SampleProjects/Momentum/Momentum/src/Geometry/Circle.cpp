//
//  Circle.cpp
//  Momentum
//
//  Created by Halil Ibrahim Kasapoglu on 17.05.2021.
//

#include "Circle.hpp"
#include <math.h>
#include <iostream>

Circle::Circle(){
    origin = Vec2();
    radius = 0;
}

Circle::Circle(Vec2 o , float r){
    colorData = new float[3]{0.5f,0.5f,0.5f};
    mass = r;
    origin = o;
    radius = r;
}


float Circle::Area(){
    return M_PI * radius * radius;
}

float Circle::GetRadius(){
    return radius;
}
void Circle::SetRadius(float r){
    radius = r;
}

Vec2 Circle::GetOrigin(){
    return origin;
}
void Circle::SetOrigin(Vec2 o){
    origin = o;
}

void Circle::SetColorData(float *cd){
    colorData = cd;
}

float * Circle::GetColorData(){
    return colorData;
}

unsigned int * Circle::indexData(){
    return indexData(DEFAULT_CIRCLE_SMOOTHNESS);
}

unsigned int * Circle::indexData(const unsigned int vertexCnt){
 
    unsigned int * indices = (unsigned int *)malloc(sizeof(unsigned int) * vertexCnt * 3);
    for (int i = 0; i < vertexCnt; i ++){
        indices[3 * i] = 0;
        indices[3 * i + 1] = (i + 1);
        indices[3 * i + 2] = (i + 2) > vertexCnt ? 1 : (i + 2);
    }
    return indices;
}


float * Circle::vertexData(){
    return vertexData(DEFAULT_CIRCLE_SMOOTHNESS);
}

float * Circle::vertexData(const unsigned int vertexCnt){
    
    float * vertices = (float *)malloc(sizeof(float) * (vertexCnt + 1) * 2);
    vertices[0] = 0;origin.getX();
    vertices[1] = 0;origin.getY();
    
    double angle = 0;
    
    for (int i = 1; i <= vertexCnt; i ++){
        Vec2 direction = Vec2(sin(angle) , cos(angle));
        Vec2 vertexPos = origin + direction * radius;
        vertices[2 * i] = vertexPos.getX();
        vertices[2 * i + 1] = vertexPos.getY();
        angle += 2 * M_PI / vertexCnt;
    }
    
    return vertices;
}

void Circle::Move(float timeScale){
    SetOrigin(Vec2(origin.getX() + velocity.getX() * timeScale , origin.getY() + velocity.getY() * timeScale));
}

unsigned int * Circle::CircleIndexData(){
    
    unsigned int * indices = (unsigned int *)malloc(sizeof(unsigned int) * DEFAULT_CIRCLE_SMOOTHNESS * 3);
    for (int i = 0; i < DEFAULT_CIRCLE_SMOOTHNESS; i ++){
        indices[3 * i] = 0;
        indices[3 * i + 1] = (i + 1);
        indices[3 * i + 2] = (i + 2) > DEFAULT_CIRCLE_SMOOTHNESS ? 1 : (i + 2);
    }
    return indices;
}

float * Circle::CircleVertexData(){
    
    float * vertices = (float *)malloc(sizeof(float) * (DEFAULT_CIRCLE_SMOOTHNESS + 1) * 2);
    vertices[0] = 0;
    vertices[1] = 0;
    
    double angle = 0;
    
    for (int i = 1; i <= DEFAULT_CIRCLE_SMOOTHNESS; i ++){
        Vec2 direction = Vec2(sin(angle) , cos(angle));
        Vec2 vertexPos = direction;
        vertices[2 * i] = vertexPos.getX();
        vertices[2 * i + 1] = vertexPos.getY();
        angle += 2 * M_PI / DEFAULT_CIRCLE_SMOOTHNESS;
    }
    
    return vertices;
}

bool Circle::CollideWith(Circle o){
    
    float eps = (1e-5);
    
    float dx = o.GetOrigin().getX() - origin.getX();
    float dy = o.GetOrigin().getY() - origin.getY();
    float distance = sqrt(dx * dx + dy * dy);
    
    return distance <= (radius + o.GetRadius() + eps);
}
bool Circle::CollideWithPoint(Vec2 p){
    float eps = (1e-5);
    float distance = (p - origin).lenght();
    return distance <= (radius + eps);
}

void Circle::ApplyElasticCollision(Circle &c1, Circle &c2){
        
    Vec2 v1 = c1.GetVelocity();
    Vec2 v2 = c2.GetVelocity();
    
    Vec2 oo = c2.GetOrigin() - c1.GetOrigin();
    
    Vec2 proj1 = v1.projOn(oo);
    Vec2 proj2 = v2.projOn(-oo);
        
    
    Vec2 com1 = v1 - proj1;
    Vec2 com2 = v2 - proj2;
    
    std::cout << com1.tostr() << " " << com2.tostr() << " " << oo.tostr() << " " << proj1.tostr() << " " << proj2.tostr() << std::endl;

    
    float m1 = c1.GetMass();
    float m2 = c2.GetMass();
    
    Vec2 proj1f = proj1*((m1 - m2)/(m1+m2)) + proj2*((2*m2)/(m1+m2));
    Vec2 proj2f = proj1*((2 * m1)/(m1+m2)) + proj2*((m2-m1)/(m1+m2));
    
    
    
    c1.SetVelocity(proj1f + com1);
    c2.SetVelocity(proj2f + com2);
    
}
