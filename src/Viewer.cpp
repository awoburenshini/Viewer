
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
    // glBindVertexArray(handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(signed int) * len, buff, GL_STATIC_DRAW);
}

GLProgramData::operator GLuint() const {
    return handle;
}


GLBackground::GLBackgroundData::GLBackgroundData() {
    glGenVertexArrays(1, &handle);
    glGenBuffers(2, vbo);
    {
        glBindVertexArray(handle);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        // interpret vbo as positions and colors pointer
#if defined(SINGLE_PRECISION)

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
#else
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), 0);
        glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (void *)(3 * sizeof(double)));
#endif
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        // render indice
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
        glBindVertexArray(0);
    }
}

void GLBackground::GLBackgroundData::uploadData(Float *data, size_t size) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Float) * size, data, GL_STATIC_DRAW);
}

void GLBackground::GLBackgroundData::uploadIndice(unsigned int *data, size_t size) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * size, data, GL_STATIC_DRAW);
}

GLBackground::GLBackgroundData::operator const GLuint() const {
    return handle;
}

void GLBackground::make_program(const std::string &vs, const std::string &fs) {
    std::ifstream vshaderSource(vs);
    if (!vshaderSource.good()) {
        std::cout << "ERROR: loading (" << vs << ") file is not good"
                  << "\n";
        throw;
    }

    std::ifstream fshaderSource(fs);
    if (!fshaderSource.good()) {
        std::cout << "ERROR: loading (" << vs << ") file is not good"
                  << "\n";
        throw;
    }
    GLShader basic_vert(GL_VERTEX_SHADER);
    basic_vert.compile(vshaderSource);
    // fragment shader
    GLShader basic_frag(GL_FRAGMENT_SHADER);
    basic_frag.compile(fshaderSource);
    // link shaders
    g_program.link(basic_vert, basic_frag);
}

GLBackground::GLBackground(const std::string &vs, const std::string &fs) {
    make_program(vs, fs);
    //position,     color
    m_data.col(0) << 1.f, 1.f, 0.0f, 0.7294, 0.8588, 0.9686;
    m_data.col(1) << 1.f, -1.f, 0.0f, 0.5922, 0.6275, 0.6314;
    m_data.col(2) << -1.f, -1.f, 0.0f, 0.5922, 0.6275, 0.6314;
    m_data.col(3) << -1.f, 1.f, 0.0f, 0.7294, 0.8588, 0.9686;
    m_indice.col(0) << 0, 1, 3;
    m_indice.col(1) << 1, 2, 3;

    g_data.uploadData(m_data.data(), m_data.size());
    g_data.uploadIndice(m_indice.data(), m_indice.size());
}

void GLBackground::draw() {
    glUseProgram(g_program);
    glBindVertexArray(g_data);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
