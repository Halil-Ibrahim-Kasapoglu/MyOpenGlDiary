#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui.h"

#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Circle.hpp"

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;
const char * WINDOW_TITLE = "MOMENTUM";
GLFWwindow *window;

std::vector < Circle > balls;
Vec2 mouse_position;

auto stclock = std::chrono::system_clock::now();
float deltaTime;
float timeScale = 0.0f;

int selectedBall = -1;

void mouseButtonCallback(GLFWwindow * window, int button ,int action , int mods){
    
    if (button == GLFW_MOUSE_BUTTON_RIGHT and action == GLFW_PRESS){
        selectedBall = -1;
        for (int i = 1; i <= balls.size(); i ++){
            if (balls[i-1].CollideWithPoint(mouse_position)){
                std::cout << std::to_string(i) << " touched\n";
                selectedBall = i-1;
                // select the first one clicked;
                break;
            }
        }
        std::cout<<selectedBall<<std::endl;
    }
    
}

static void cursorPositionCallback(GLFWwindow * window, double xPos , double yPos){
    std::cout << xPos << " " << yPos << std::endl;
    
    //normalized x and y between -1 to 1
    double nx = (xPos - WINDOW_WIDTH/2.0f)/(WINDOW_WIDTH/2.0f);
    double ny = -(yPos - WINDOW_HEIGHT/2.0f)/(WINDOW_HEIGHT/2.0f);
    //normalized touch point np
    mouse_position = Vec2(nx,ny);
    

    
}

int loadWindow(){
    if ( !glfwInit() ){
        fprintf(stderr, "failed to init glew");
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (window == NULL){
        fprintf(stderr, "failed to open window");
        glfwTerminate();
        return -1;
    }
    
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwMakeContextCurrent(window);
    
    glewExperimental = true;
    if (glewInit() != GLEW_OK){
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    return 0;
}

void loadBallInspector(int i){

    Vec2 speed = balls[i].GetVelocity();
    float velocityData[2] = {speed.getX() , speed.getY()};
    //float limit = fmax(1.0f , fmax(abs(speed.getX()) , abs(speed.getY())));
    float *colorData = balls[i].GetColorData();
    float positionData[2] = {balls[i].GetOrigin().getX() , balls[i].GetOrigin().getY()};
    float mass = balls[i].GetMass();
    float radius = balls[i].GetRadius();
    ImGui::Text("%s", ("Selected Ball : " + std::to_string(i+1)).c_str());
    ImGui::SliderFloat2("Position", &positionData[0] , -1, 1);
    ImGui::SliderFloat2("Velocity", &velocityData[0] , -1, 1);
    ImGui::ColorEdit3("color", &colorData[0]);
    ImGui::SliderFloat("mass" ,&mass, 0.1, 1.0);
    ImGui::SliderFloat("radius" ,&radius, 0.1, 0.5f);

    balls[i].SetMass(mass);
    balls[i].SetRadius(radius);
    balls[i].SetColorData(&colorData[0]);
    balls[i].SetVelocity(Vec2(velocityData[0],velocityData[1]));
    balls[i].SetOrigin(Vec2(positionData[0],positionData[1]));
}

void IMGUI(){
    
    ImGui::Begin("Momentum");
    if (ImGui::Button("New Ball")){
        Circle ball = Circle(Vec2(0,0), 0.20);
        balls.push_back(ball);
    }
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
    
    std::vector<bool> deleted(balls.size());
    
    ImGui::Begin("Inspector");
    if (selectedBall!=-1){
        loadBallInspector(selectedBall);
        if (ImGui::Button("Delete")){
            deleted[selectedBall] = true;
            selectedBall = -1;
        }
    }
    ImGui::End();
    
    
    //    for (int i = 0; i < balls.size(); i ++){
    //        ImGui::Begin("Ball");
    //        loadBallInspector(i);
    //        if (ImGui::Button("Delete")){
    //            deleted[i] = true;
    //        }
    //        ImGui::End();
    //    }
    
    // delete
    std::vector <Circle> tmpBalls = balls;
    balls.clear();
    for (int i = 0; i < tmpBalls.size(); i++){
        if (!deleted[i]){
            balls.push_back(tmpBalls[i]);
        }
    }
}

void Update(){
    
    //std::cout << deltaTime << "\n";
    
    
    if (abs(timeScale) <= 1e-3) return;
    
    for (int i = 0; i < balls.size(); i ++){
        for (int j = i + 1; j < balls.size(); j ++){
            if (balls[i].CollideWith(balls[j])){
                Circle().ApplyElasticCollision(balls[i], balls[j]);
                std::cout << "Collision: " << "[" << i << ", " << j << "]" << "\n";
            }else {
                std::cout << "NO COLLISION" << "\n";
            }
        }
        
        balls[i].Move(deltaTime * timeScale);
    }
    
    
}

int main(int argc, const char * argv[]) {
    
    if (loadWindow() == -1){
        std::cout << "cannot load window\n";
        return -1;
    }
    
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char * glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    VertexArray va;
    VertexBuffer vb(Circle().CircleVertexData(), sizeof(float) * (DEFAULT_CIRCLE_SMOOTHNESS+1)*2 );
    IndexBuffer ib(Circle().CircleIndexData() , DEFAULT_CIRCLE_SMOOTHNESS * 3);
    
    
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    
    Shader shader("res/VertexShader.shader", "res/FragmentShader.shader");
    
    Renderer renderer;
    
    while (!glfwWindowShouldClose(window)){
        
        
        timeScale = 0.0f;
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_TRUE){
            timeScale = 0.5f;
        }else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_TRUE){
            timeScale = 0.05f;
        }else if (glfwGetKey(window, GLFW_KEY_U) == GLFW_TRUE){
            timeScale = -0.5f;
        }else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_TRUE){
            timeScale = -0.05f;
        }
        
        renderer.Clear();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        IMGUI();
        
        Update();
        
        for (int i = 0; i < balls.size(); i ++){
            Circle ball = balls[i];
            shader.SetUniform4f("transform", ball.GetOrigin().getX(), ball.GetOrigin().getY(), 0, 0);
            shader.SetUniform4f("scale", ball.GetRadius(), ball.GetRadius(), 0, 0);
            shader.SetUniform4f("bg_color", ball.GetColorData()[0], ball.GetColorData()[1], ball.GetColorData()[2], 1.0);
            renderer.Draw(va, ib, shader);
        }
        
        
        auto time = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = time-stclock;
        deltaTime = elapsed_seconds.count();
        stclock = time;
        
        
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        
        
    }
    
    glfwTerminate();
    
    
    return 0;
}

