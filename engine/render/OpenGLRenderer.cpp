#include "OpenGLRenderer.h"
#include "SDL_video.h"
#include "core/Engine.h"
#include "core/Log.h"
#include <iostream>
#include <string>
#include "glad/glad.h"

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

unsigned int program, VAO, VBO;

OpenGLRenderer::OpenGLRenderer(SDL_Renderer* renderer) 
    : sdlRenderer(renderer), dirtyList(true) {}

void OpenGLRenderer::init() {
    
    if (SDL_GL_MakeCurrent(window, SDL_GL_GetCurrentContext()) != 0) {
        gameLog("Failed to make GL context current: " + std::string(SDL_GetError()), ERROR);
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        gameLog("Failed to load GL functions", ERROR);
    }
    
    gameLog("OpenGLRenderer Initialized successfully.", INFO);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    int success;
    char infoLog[512];
    
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog <<
        std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog <<
        std::endl;
    }

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetShaderiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infoLog <<
        std::endl;
    }

    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
}

void OpenGLRenderer::beginFrame() {
    glClearColor(0.05, 0.1, 0.08, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderer::drawScene(std::vector<std::unique_ptr<GameObject>>& objects, const Camera& camera) {    
    glUseProgram(program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    if (objects.empty()) return;
}

void OpenGLRenderer::endFrame() {
    SDL_GL_SwapWindow(window);
    dirtyList = false;
}