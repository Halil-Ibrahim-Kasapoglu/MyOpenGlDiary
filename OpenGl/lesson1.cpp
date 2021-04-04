#include <bits/stdc++.h>
#include <GL/glew.h>
#include <GLFW/glfw3>

using namespace glm;

int main(){

	glewExperimental = true;
	if (!glfwInit()){
		fprintf(stderr, "Failed to initialize GLFW\n");
	}

}