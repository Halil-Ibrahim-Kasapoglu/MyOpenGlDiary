#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui.h"

const unsigned int window_width = 600;
const unsigned int window_height = 600;
const char * window_title = "OpenGl";

GLFWwindow* window;

unsigned int shaderProgram;
unsigned int buffer , VAO , EBO;

struct ColorData{
    float r, g , b , a;
    
    ColorData(int red , int green , int blue){
        r = red / 255.0f;
        g = green / 255.0f;
        b = blue / 255.0f;
        a = 1.0f;
    }
    
    ColorData(float red , float green , float blue){
        r = red;
        g = green;
        b = blue;
        a = 1.0f;
    }
};

std::string readShaderFile(std::string filePath){
    
    std::ifstream fileStream(filePath);
    std::string src = "";
    std::string line;
    while (getline(fileStream, line)){
        src += line + "\n";
    }
    fileStream.close();
    return src;
}

void loadShaders(){
    
//    std::string x = readShaderFile("res/shaders/VertexShader.shader");
//    std::string y = readShaderFile("res/shaders/FragmentShader.shader");
    
    std::string vertexShaderSourceString = readShaderFile("res/shaders/VertexShader.shader");
    std::string fragmentShaderSourceString = readShaderFile("res/shaders/FragmentShader.shader");
    const char * vertexShaderSource = vertexShaderSourceString.c_str();
    const char * fragmentShaderSource = fragmentShaderSourceString.c_str();
    
    std::cout << " -> " << vertexShaderSource << "\n";
    
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader , 1 , &vertexShaderSource , NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader , GL_COMPILE_STATUS , &success);
    if (!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader , 1 , & fragmentShaderSource , NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader , GL_COMPILE_STATUS , &success);
    if (!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram , vertexShader);
    glAttachShader(shaderProgram , fragmentShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram , GL_COMPILE_STATUS , &success);
    if (!success){
        glGetProgramInfoLog(shaderProgram , 512 , NULL , infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
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
    

    
    window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
    if (window == NULL){
        fprintf(stderr, "failed to open window");
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK){
        fprintf(stderr, "Failed to initialize GLEW\n");
         return -1;
    }
    return 0;
}

void setVerticesForNGonWithRadius(float* vertices , int n , float radius , ColorData color , float initialRotation = 0 ){
    
    float degreeStep = 360.0f / n;
    
    // center point
    vertices[0] = vertices[1] = vertices[2] = 0.0f;
    vertices[3] = vertices[4] = vertices[5] = 0.0f;

    float degree = initialRotation;
    for (int i = 1 ; i <= n ; i ++ , degree += degreeStep){
        float x , y , z = 0.0f;
        x = radius * cos(degree * M_PI / 180.0f);;
        y = radius * sin(degree * M_PI / 180.0f);;
        
        // position
        vertices[6 * i + 0] = x;
        vertices[6 * i + 1] = y;
        vertices[6 * i + 2] = z;
        
        //color
        vertices[6 * i + 3] = color.r;// * (1.0 +(rand()%200 / 500.0f) - 1/5.0f);
        vertices[6 * i + 4] = color.g;// * (1.0 +(rand()%200 / 500.0f) - 1/5.0f);
        vertices[6 * i + 5] = color.b;// * (1.0 +(rand()%200 / 500.0f) - 1/5.0f);

    }
}

void loadVertexArrayObjectForNGon(int n , float radius , ColorData color , float rotation = 0.0f ){
    
    float vertices[(n+1)*6];
    setVerticesForNGonWithRadius(vertices, n, radius , color , rotation );
    
    // get proper indices of triangles for n-gon
    unsigned int indices[n * 3];
    for (int i = 0; i < n; i ++){
        indices[3 * i + 0] = 0;
        indices[3 * i + 1] = i + 1;
        indices[3 * i + 2] = i + 2 > n ? 1 : i + 2;
    }
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT , GL_FALSE , 6 * sizeof(float)  , (void *)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(VAO);
    
}


int main() {
    
    if (loadWindow() == -1)return -1;
    loadShaders();
    
    glGenBuffers(1 , &EBO);
    glGenBuffers(1 , &buffer);
    glGenVertexArrays(1 , &VAO);
    
    float rotation = 0.0f;
    auto startTime = std::chrono::system_clock::now();
    float rotationSpeed = 0.0f; // over seconds;
    
    
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char * glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    //int counter = 0;
    
    int vertexCnt = 3;
    float colorSpeed = 1.0f;
    float colorTime = 0.0f;
    float radius = 0.5f;
    
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    do{
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1, 0.0, 0.15, 1);
        
        // IMGUI SECTION BEGINS
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::Begin("NGon-Master");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("You can edit ngon");               // Display some text (you can use a format strings too)
        //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        //ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderInt("Vertex Count", &vertexCnt, 3, 50);
        ImGui::SliderFloat("Rotation Speed (radian / per second)", &rotationSpeed, -2.0f, 2.0f);
        ImGui::SliderFloat("Color Speed" , &colorSpeed , 0.0f , 10.0f);
        ImGui::SliderFloat("Radius" , &radius , 0.0f , 0.75f);

        //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
        
//        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//            counter++;
//        ImGui::SameLine();
//        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        
        // IMGUI SECTION ENDS
        
        
        
        
        
        auto time = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = time-startTime;
        float rotationInRadians = ( elapsed_seconds.count() * rotationSpeed * M_PI );
        startTime = time;
        rotation += rotationInRadians * 180.0f / M_PI;
        colorTime += elapsed_seconds.count() * colorSpeed;
        
        float greenValue = abs(sin(colorTime /2));
        float redValue = abs(sin(colorTime /3));
        float blueValue = abs(sin(colorTime /5));
        
        loadVertexArrayObjectForNGon(vertexCnt , radius , ColorData(redValue, greenValue, blueValue) , rotation);
        
        int vertexColorLocation = glGetUniformLocation(shaderProgram , "color");
        glUniform4f(vertexColorLocation , redValue , greenValue , blueValue , 1.0f);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3 * (vertexCnt), GL_UNSIGNED_INT , 0);
    
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS and glfwWindowShouldClose(window) == 0);
    
    
    return 0;
}
