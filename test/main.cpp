
#include <extensionViewer.h>
#include <iostream>
#include <fstream>
#include <string>

#include <GLFW/glfw3.h>
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
Float vertices[] = {
    .5f, .5f, 0.0f,   // top right
    .5f, -.5f, 0.0f,  // bottom right
    -.5f, -.5f, 0.0f, // bottom left
    -.5f, .5f, 0.0f   // top left
};

unsigned int indices[] = {
    // note that we start from 0!
    3, 1, 0, // first Triangle
    1, 2, 3  // second Triangle
};

// settings
const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

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

int main() {

    GLFWwindow *window = initWindow();
    GLBackground background(
        "D:/Viewer/shader/background.vs",
        "D:/Viewer/shader/background.fs");
    // build and compile our shader program
    // ------------------------------------
    // GLProgram easyProgram;
    learnOpenGLProgram easyProgram;
    Camera cam;
    Vector3r origin(1.f, 1.5f, 1.f);
    Vector3r target(0.f, 0.f, 0.f);
    Vector3r up(0.f, 0.f, 1.f);

    cam.setLookAt(origin, target, up);
    cam.setProjective(-0.01, 0.01, -0.01, 0.01, 0.01, 100);
    // cam.setOrtho(-0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 100.0f);
    makeProgram(
        easyProgram,
        "D:/Viewer/shader/learnOpenGL.vs",
        "D:/Viewer/shader/learnOpenGL.fs");

    easyProgram.setOurColor({0.6314, 0.0706, 0.0706, 0.877});

    std::cout << "projective: \n"
              << cam.getProjectiveMatrix() << std::endl;
    std::cout << cam.getProjectiveMatrix() << std::endl;
    easyProgram.setProjective(cam.getProjectiveMatrix());

    std::cout << "view: \n"
              << cam.getViewMatrix() << std::endl;
    easyProgram.setView(cam.getViewMatrix());

    GLModelData GLData;
    std::cout<< sizeof(vertices)<< std::endl;
    GLData.setPositionData(vertices, sizeof(vertices) / 4);
    GLData.setIndexData(indices, 6);

    // uncomment this call to draw in wireframe polygons.

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        processInput(window);

        // render
        // ------
        // glClearColor(0.f, 0.f, 0.f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        background.draw();
        
        // use easyProgram draw GLData
        {
            glUseProgram(easyProgram);
            glBindVertexArray(GLData); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0); // no need to unbind it every time
            glUseProgram(0);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
