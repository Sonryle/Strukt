#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <client/logging.h>
#include <client/settings.h>
#include <client/renderer.h>
#include <client/window.h>

int create_shader(GLenum shader_type, const char* shader_path, GLuint* shader_ptr);
int build_shader_program(const char* vs_path, const char* fs_path, GLuint* program_ptr);

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};
GLuint VBO;
GLuint VAO;
GLuint shader_program;

int init_renderer()
{
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
                log_err("failed to initialize GLAD");
                glfwTerminate();
                return -1;
        }

        glClearColor(settings.renderer->bgr, settings.renderer->bgg, settings.renderer->bgb, 1.0f);
        glViewport(0, 0, settings.window->initial_width, settings.window->initial_height);

        // Create Buffers
        // --------------

        /* Generate a buffer to store our vertices */
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);

        /* Bind Vertex Array Buffer */
        glBindVertexArray(VAO);

        /* Bind buffer to GL_ARRAY_BUFFER buffer type */
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        /* Copy user-defined data into currently bound buffer of type GL_ARRAY_BUFFER */
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

        build_shader_program("shaders/source.vs", "shaders/source.fs", &shader_program);
        glUseProgram(shader_program);

        // Linking vertex attributes
        // -------------------------

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        return 0;
}

int build_shader_program(const char* vs_path, const char* fs_path, GLuint* program_ptr)
{
        GLuint vertex_shader;
        GLuint fragment_shader;
        if (create_shader(GL_VERTEX_SHADER, vs_path, &vertex_shader) != 0)
        {
                log_err("could not build shader program");
                return -1;
        }
        if (create_shader(GL_FRAGMENT_SHADER, fs_path, &fragment_shader) != 0)
        {
                log_err("could not build shader program");
                return -1;
        }

        *program_ptr = glCreateProgram();
        glAttachShader(*program_ptr, vertex_shader);
        glAttachShader(*program_ptr, fragment_shader);
        glLinkProgram(*program_ptr);
        int success;
        char info_log[512];
        glGetProgramiv(*program_ptr, GL_LINK_STATUS, &success);
        if (!success)
        {
                glGetProgramInfoLog(*program_ptr, 512, NULL, info_log);
                log_err("could not link shader program:\n%s", info_log);
                return -1;
        }
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return 0;
}

int create_shader(GLenum shader_type, const char* shader_path, GLuint* shader_ptr)
{
        /* Find & copy vertex shader source */
        FILE* shader_file = fopen(shader_path, "r");
        char* shader_src = NULL;

        if (shader_file == NULL)
        {
                if (shader_type == GL_VERTEX_SHADER)
                        log_err("could not find vertex shader file");
                else if (shader_type == GL_FRAGMENT_SHADER)
                        log_err("could not find fragment shader file");
                else
                        log_err("could not find shader file");
                return -1;
        } else
        {
                fseek(shader_file, 0, SEEK_END);
                int char_count = ftell(shader_file);

                shader_src = (char*)malloc((char_count + 1) * sizeof(char));
                if (shader_src)
                {
                        fseek(shader_file, 0, SEEK_SET);
                        fread((void*)shader_src, sizeof(char), char_count, shader_file);
                        shader_src[char_count] = '\0';
                }
                fclose(shader_file);
        }

        *shader_ptr = glCreateShader(shader_type);
        glShaderSource(*shader_ptr, 1, (const GLchar**)&shader_src, NULL);
        glCompileShader(*shader_ptr);
        free(shader_src);

        int success;
        char info_log[512];
        glGetShaderiv(*shader_ptr, GL_COMPILE_STATUS, &success);
        if (!success)
        {
                glGetShaderInfoLog(*shader_ptr, 512, NULL, info_log);
                log_err("could not compile vertex shader:\n%s", info_log);
                return -1;
        }

        return 0;
}

int temporary_render()
{
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        return 0;
}

void terminate_renderer()
{

}
