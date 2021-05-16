#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Windows.h>
using namespace std;

void processInput(GLFWwindow* window);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

    if (window == NULL) {
        std::cout << "Fail to creat GLFW windows" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // load all 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	int i = 0;
    while (!glfwWindowShouldClose(window)) {

		//Sleep(500);




		//Sleep(500);
		cout << "processInput "<< i++ << endl;

        processInput(window);

		//Sleep(500);
		cout << "glClearColor " << i++ << endl;


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		//Sleep(500);
		cout << "glClear " << i++ << endl;


        glClear(GL_COLOR_BUFFER_BIT);

		//Sleep(500);
		cout << "glfwSwapBuffer " << i++ << endl;

        glfwSwapBuffers(window);

		cout << "glfwPollEvents " << i++ << endl;

		glfwPollEvents();


		Sleep(500);
		cout << "end " << i++ << endl;
    }
    

    glfwTerminate();

    return 0;
}


void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        //glfwSetWindowShouldClose(window, true);
		cout << "process!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    printf("viewport changed");
}