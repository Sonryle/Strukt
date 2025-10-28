#pragma once

int init_renderer(GLuint* VBO, GLuint* VAO, GLuint* shader_program, const char* vshader_path, const char* fshader_path);
int temporary_render(GLuint* VAO, GLuint* shader_program, GLFWwindow* window);
void terminate_renderer(GLuint* shader_program);
