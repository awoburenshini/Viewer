#include <MeshIO.h>
#include <iostream>
#include <fstream>
#include <string>
#include <extensionViewer.h>
#include <vector>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 800;

GLFWwindow *initWindow() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }
    return window;
}

void makeProgram(GLProgram &glprogram, const std::string &vs_path, const std::string &fs_path) {
    std::ifstream vshaderSource(vs_path);
    if (!vshaderSource.good()) {
        std::cout << "ERROR: loading (" << vs_path << ") file is not good"
                  << "\n";
        throw;
    }

    std::ifstream fshaderSource(fs_path);
    if (!fshaderSource.good()) {
        std::cout << "ERROR: loading (" << vs_path << ") file is not good"
                  << "\n";
        throw;
    }

    // vertex shader
    GLShader basic_vert(GL_VERTEX_SHADER);
    basic_vert.compile(vshaderSource);
    // fragment shader
    GLShader basic_frag(GL_FRAGMENT_SHADER);
    basic_frag.compile(fshaderSource);
    // link shaders
    glprogram.link(basic_vert, basic_frag);
}

std::vector<unsigned int> range(int start, int length){
    std::vector<unsigned int> rg(length,0);
    for(int i = 0 ; i < length; ++i){
        rg[i] = i;
    }
    return rg;
}

int main() {
    GLFWwindow *window = initWindow();

    Camera cam;
    Vector3r origin(4.f, 6.f, 4.f);
    Vector3r target(0.f, 0.f, 0.f);
    Vector3r up(0.f, 0.f, 1.f);

    cam.setLookAt(origin, target, up);
    cam.setProjective(-0.01, 0.01, -0.01, 0.01, 0.01, 100);

    MatrixXr V;
    MatrixXu F;
    load_obj("../model/cube.obj", V, F);
    
    GLModelData GLData;
    std::vector<unsigned int> IF = range(0,V.cols());

    GLData.setPositionData(V.data(), V.size());
    GLData.setIndexData(IF.data(), IF.size());

    learnOpenGLProgram pickerProgram;
    makeProgram(
        pickerProgram,
        "../shader/pickerShader.vs",
        "../shader/pickerShader.fs");
    pickerProgram.setProjective(cam.getProjectiveMatrix());
    pickerProgram.setView(cam.getViewMatrix());
    
    // learnOpenGLProgram easyProgram;
    // makeProgram(
    //     easyProgram,
    //     "D:/Viewer/shader/learnOpenGL.vs",
    //     "D:/Viewer/shader/learnOpenGL.fs");
    // easyProgram.setProjective(cam.getProjectiveMatrix());
    // easyProgram.setView(cam.getViewMatrix());
    


    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        {
            pickerProgram.setOurColor({0.6314, 0.0706, 0.0706, 0.877});
            glUseProgram(pickerProgram);
            glBindVertexArray(GLData);
            // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glPointSize(10.0);
            glDrawElements(GL_POINTS, IF.size(), GL_UNSIGNED_INT, NULL);
            // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBindVertexArray(0);
            glUseProgram(0);
        }
        // {
        //     easyProgram.setOurColor({0., 1., 0.0706, 0.877});
        //     glUseProgram(easyProgram);
        //     glBindVertexArray(GLData);
        //     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //     glDrawElements(GL_TRIANGLES, F.size(), GL_UNSIGNED_INT, NULL);
        //     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //     glBindVertexArray(0);
        //     glUseProgram(0);
        // }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}



void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        std::cout << "hello" << std::endl;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        std::cout << "hello"<< xpos<< " "<< ypos << std::endl;
    }
}
