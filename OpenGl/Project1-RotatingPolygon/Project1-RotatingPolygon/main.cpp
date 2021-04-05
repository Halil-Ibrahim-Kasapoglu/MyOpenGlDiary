#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


const GLint window_width = 512;
const GLint window_height = 512;
const char * window_title = "OpenGl";

GLFWwindow* window;

unsigned int shaderProgram;

unsigned int buffer , VAO , EBO;


const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.75f, 0.45f, 0.05f, 1.0f);\n"
    "}\n\0";

/*
 params
 0<= r , g , b <= 255
 */
const char * getFragmentShaderSourceWithColor(int r , int g , int b){
    // implement later
    return "";
}

void loadShaders(){
    
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

void setVerticesForNGonWithRadius(float* vertices , int n , float radius , float initialRotation = 0){
    
    float degreeStep = 360.0f / n;
    
    // center point
    vertices[0] = vertices[1] = vertices[2] = 0.0f;
    
    float degree = initialRotation;
    for (int i = 1 ; i <= n ; i ++ , degree += degreeStep){
        float x , y , z = 0.0f;
        x = radius * cos(degree * M_PI / 180.0f);;
        y = radius * sin(degree * M_PI / 180.0f);;
        vertices[3 * i + 0] = x;
        vertices[3 * i + 1] = y;
        vertices[3 * i + 2] = z;
    }
}

void loadVertexArrayObjectForNGon(int n , float rotation = 0.0f){
    
    float vertices[(n+1)*3];
    setVerticesForNGonWithRadius(vertices, n, 0.75f , rotation);
    
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
    
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(VAO);
    
}


int main() {
    
    int vertexCnt = 0;
    
    while (vertexCnt < 3){
        try {
            std::cout << "Enter Vertex Count (n>=3) \n";
            std::cin >> vertexCnt;
        } catch (int x) {
            std::cout << "error";
        }
        if (vertexCnt < 3){
            std::cout << "vertex count must be at least 3\n";
            
        }
    }
        
    if (loadWindow() == -1)return -1;
    loadShaders();
    
    glGenBuffers(1 , &EBO);
    glGenBuffers(1 , &buffer);
    glGenVertexArrays(1 , &VAO);
    
    float rotation = 0.0f;
    auto startTime = std::chrono::system_clock::now();
    float rotationSpeed = M_PI / 4; // over seconds;
    
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    do{
        glClear(GL_COLOR_BUFFER_BIT);
    
        auto time = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = time-startTime;
        float rotationInRadians = ( elapsed_seconds.count() * rotationSpeed );
        startTime = time;
        rotation += rotationInRadians * 180.0f / M_PI;
        
        loadVertexArrayObjectForNGon(vertexCnt , rotation);
        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 4);
        glDrawElements(GL_TRIANGLES, 3 * (vertexCnt), GL_UNSIGNED_INT , 0);
    
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS and glfwWindowShouldClose(window) == 0);
    
    
    return 0;
}
