
#include <Viewer.h>
#include <iostream>

GLShader::GLShader(GLenum shaderType) :
    handle(glCreateShader(shaderType)){};

GLShader::~GLShader() {
    glDeleteShader(handle);
};

void GLShader::compile(const char *source) {
    GLint compiled     = 0;
    const char *ptrs[] = {source};
    const GLint lens[] = {(GLint)std::strlen(source)};
    glShaderSource(handle, 1, ptrs, lens);
    glCompileShader(handle);
    glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint logSize = 0;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logSize);
        std::vector<GLchar> errorLog(logSize);
        glGetShaderInfoLog(handle, logSize, &logSize, &errorLog[0]);
        std::cerr << &errorLog[0] << std::endl;
        throw std::runtime_error("Failed to compile shader.");
    }
};

void GLShader::compile(std::ifstream &source) {
    std::vector<char> text;
    source.seekg(0, std::ios_base::end);
    std::streampos fileSize = source.tellg();
	if (fileSize == -1) {
        std::cerr << "reading fault, fileSize is -1" << std::endl;
        throw;
	}
    text.resize((size_t)fileSize);

    source.seekg(0, std::ios_base::beg);
    source.read(&text[0], fileSize);
    compile(&text[0]);
}

GLShader::operator const GLuint() const {
    return handle;
}

GLProgram::GLProgram() :
    handle(glCreateProgram()) {
}
void GLProgram::postLink(){
}

void GLProgram::link(const GLShader &vshader, const GLShader &fshader) {
    GLint linked = 0;
    glAttachShader(handle, vshader);
    glAttachShader(handle, fshader);
    glLinkProgram(handle);
    glGetProgramiv(handle, GL_LINK_STATUS, &linked);
    if (!linked) {
        throw std::runtime_error("Fail to link shader.");
    }
    postLink();
}

GLProgram::operator const GLuint() const {
    return handle;
}

GLProgram::~GLProgram() {
    glDeleteProgram(handle);
}

GLProgramData::GLProgramData() {
    // generate vertex array(VAO) pointer
    glGenVertexArrays(1, &handle);
    // generate buffer(VBO) pointer
    glGenBuffers(2, &vbo[0]);
    {
        glBindVertexArray(handle);
        // interpret vbo[0] as positions pointer
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
#if defined(SINGLE_PRECISION)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
#else
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, 0);
#endif
        glEnableVertexAttribArray(0);
        // render indice
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
        glBindVertexArray(0);
    }

}

GLProgramData::~GLProgramData() {
    glDeleteBuffers(4, vbo);
    glDeleteVertexArrays(1, &handle);
}

void GLProgramData::bufferData(unsigned int index, void *buff, size_t size) {
    //glBindVertexArray(handle);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[index]);
    glBufferData(GL_ARRAY_BUFFER, size, buff, GL_STATIC_DRAW);
}

void GLProgramData::setPositionData(Float *buff, unsigned int len) {
    bufferData(0, buff, sizeof(Float) * len);
}

void GLProgramData::setIndexData(unsigned int *buff, unsigned int len) {
    glBindVertexArray(handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(signed int) * len, buff, GL_STATIC_DRAW);
}

GLProgramData::operator GLuint() const {
    return handle;
}


