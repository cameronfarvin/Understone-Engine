#include <engine_tools/ogl_tools.h>

void
glErrorFileLine(const char* file_name,
                int line_number,
                const char* function_name)
{
    char* error_type_string = (char*) calloc(35, sizeof(char));
    bool print_warning = true;
    switch(glGetError()) 
    { 
        case GL_NO_ERROR: 
            { 
                print_warning = false;
                break;
            } 
        case GL_INVALID_ENUM: 
            {
                strcpy(error_type_string, "GL_INVALID_ENUM");
                break;
            } 
        case GL_INVALID_VALUE: 
            {
                strcpy(error_type_string, "GL_INVALID_VALUE");
                break;
            } 
        case GL_INVALID_OPERATION: 
            { 
                strcpy(error_type_string, "GL_INVALID_OPERATION");
                break;
            }                                                               
        case GL_INVALID_FRAMEBUFFER_OPERATION:                          
            {                                              
                strcpy(error_type_string, "GL_INVALID_FRAMEBUFFER_OPERATION");
                break;
            }                                                               
        case GL_OUT_OF_MEMORY:                                          
            {                                                          
                strcpy(error_type_string, "GL_OUT_OF_MEMORY");
                break;
            }                                                               
        default: { assert(0); }                                         
    }

    if (print_warning)
    {
        printf("[ GLERROR ] %s\n\tFILE: %s\n\tLINE: %d\n\tFUNCTION: %s\n",
               error_type_string,
               file_name,
               line_number,
               function_name);                
        if (error_type_string) { free(error_type_string); }
        assert(0);
    }

    if (error_type_string) { free(error_type_string); }
}

void
uGLCheckErrorState(GLuint object,
                   GLenum parameter_to_check,
                   const char* pipeline_state,
                   const char* file_name)
{
    GLint   err = GL_TRUE;
    GLsizei log_length = 0;

    char* message = (char*) calloc(1024, sizeof(char));
    char* compile_link = (char*) calloc(35, sizeof(char));

    glError;
    glGetShaderiv(object, parameter_to_check, &err);
    glError;
    glGetShaderInfoLog(object, 1024, &log_length, message);
    glError;


    if (err != GL_TRUE)
    {
        switch (parameter_to_check)
        {
            case GL_COMPILE_STATUS:
                {
                    strcpy(compile_link, "GL_COMPILE_STATUS");
                    break;
                }
            case GL_LINK_STATUS:
                {
                    strcpy(compile_link, "GL_LINK_STATUS");
                    break;
                }
        }

        printf("[ ERROR | %s | %s ] FILE: %s\n%s",
               compile_link,
               pipeline_state,
               file_name,
               message);

        fflush(stdout);
        if (message) { free(message); }
        if (compile_link) { free(compile_link); }
        message = NULL;
        assert(false);
    }

    if (message) { free(message); }
    if (compile_link) { free(compile_link); }
}

GLuint
uGLCreateShaderProgram_vf(const GLchar** vertex_shader_source,
                          const GLchar** fragment_shader_source,
                          const char*    file_name)
{
    // create, compile & error check vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    assert(vertex_shader);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    uGLCheckErrorState(vertex_shader,
                       GL_COMPILE_STATUS,
                       "VERTEX_SHADER",
                       file_name);

    // create, compile & error check fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    assert(fragment_shader);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    uGLCheckErrorState(fragment_shader,
                       GL_COMPILE_STATUS,
                       "FRAGMENT_SHADER",
                       file_name);

    // create, attach vert/frag, link & error check shader program
    GLuint shader_program = glCreateProgram();
    assert(shader_program);
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    uGLCheckErrorState(shader_program,
                       GL_LINK_STATUS,
                       "SHADER_PROGRAM",
                       file_name);

    // delete shaders, return glShaderProgram identifier
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

GLuint
uGLCreateShaderProgram_vgf(const GLchar** vertex_shader_source,
                           const GLchar** geometry_shader_source,
                           const GLchar** fragment_shader_source,
                           const char*    file_name)
{
    // create, compile & error check vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    assert(vertex_shader);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    uGLCheckErrorState(vertex_shader,
                       GL_COMPILE_STATUS,
                       "VERTEX_SHADER",
                       file_name);

    // create, compile & error check geometry shader
    GLuint geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
    assert(geometry_shader);
    glShaderSource(geometry_shader, 1, geometry_shader_source, NULL);
    glCompileShader(geometry_shader);
    uGLCheckErrorState(geometry_shader,
                       GL_COMPILE_STATUS,
                       "GEOMETRY_SHADER",
                       file_name);

    // create, compile & error check fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    assert(fragment_shader);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    uGLCheckErrorState(fragment_shader,
                       GL_COMPILE_STATUS,
                       "FRAGMENT_SHADER",
                       file_name);

    // create, attach vert/frag, link & error check shader program
    GLuint shader_program = glCreateProgram();
    assert(shader_program);
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, geometry_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    uGLCheckErrorState(shader_program,
                       GL_LINK_STATUS,
                       "SHADER_PROGRAM",
                       file_name);

    // delete shaders, return glShaderProgram identifier
    glDeleteShader(vertex_shader);
    glDeleteShader(geometry_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

