#include <engine_tools/ogl_tools.h>


bool
uIsExtensionSupported(uDynamicArray* uDA, const char* extension_name)
{
    if (strchr(extension_name, ' ') || *extension_name == '\0')
    {
        return false;
    }

    size_t atomic_elements = uDA->length;
    for (size_t ii = 0; ii < atomic_elements; ii++)
    {
        if ((strcmp( ((uString*)uDAIndex(uDA, ii))->data, extension_name)) == 0)
        {
            return true;
        }
    }

    return false;
}


void
glErrorFileLine(const char* file_name,
                int line_number,
                const char* function_name)
{
    char* error_type_string = (char*) calloc(35, sizeof(char));
    bool print_warning = true;

#ifdef _WIN32
#pragma warning( push )
#pragma warning( disable: 4996 )
#endif

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

#ifdef _WIN32
#pragma warning( pop )
#endif

    if (print_warning)
    {
        uError_v("[ GLERROR ] %s\n\tFILE: %s\n\tLINE: %d\n\tFUNCTION: %s\n",
               error_type_string,
               file_name,
               line_number,
               function_name);
        fflush(stdout);
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
    glError;

    GLint   err = GL_TRUE;
    GLsizei log_length = 0;

    char* message = (char*) calloc(1024, sizeof(char));
    char* compile_link = (char*) calloc(35, sizeof(char));

#ifdef _WIN32
#pragma warning( push )
#pragma warning( disable: 4996 )
#endif
    switch (parameter_to_check)
    {
        case GL_COMPILE_STATUS:
        {
            glGetShaderiv(object, parameter_to_check, &err);
            glError;
            glGetShaderInfoLog(object, 1024, &log_length, message);
            glError;

            if (err != GL_TRUE)
            {
                strcpy(compile_link, "GL_COMPILE_STATUS");
            }
            break;
        }
        case GL_LINK_STATUS:
        {
            glGetProgramiv(object, parameter_to_check, &err);
            glError;
            glGetProgramInfoLog(object, 1024, &log_length, message);
            glError;

            if (err != GL_TRUE)
            {
                strcpy(compile_link, "GL_LINK_STATUS");
            }
            break;
        }
    }

#ifdef _WIN32
#pragma warning( pop )
#endif

    if (err != GL_TRUE)
    {
        uError("[ ERROR | %s | %s ] FILE: %s\n[ Error Message ]%s\n",
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
uGLCreateShaderProgram_vf_API(const GLchar** vertex_shader_source,
                              const GLchar** fragment_shader_source,
                              const char*    file_name)
{
    // create, compile & error check vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    assert(vertex_shader);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glError;
    glCompileShader(vertex_shader);
    uGLCheckErrorState(vertex_shader,
                       GL_COMPILE_STATUS,
                       "VERTEX_SHADER",
                       file_name);
    assert(glIsShader(vertex_shader));

    // create, compile & error check vertex shader
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glError;
    glCompileShader(vertex_shader);
    uGLCheckErrorState(vertex_shader,
                       GL_COMPILE_STATUS,
                       "VERTEX_SHADER",
                       file_name);
    assert(glIsShader(vertex_shader));

    // create, compile & error check fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    assert(fragment_shader);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glError;
    glCompileShader(fragment_shader);
    uGLCheckErrorState(fragment_shader,
                       GL_COMPILE_STATUS,
                       "FRAGMENT_SHADER",
                       file_name);
    assert(glIsShader(fragment_shader));

    // create, attach vert/frag, link & error check shader program
    GLuint shader_program = glCreateProgram();
    assert(shader_program);
    glAttachShader(shader_program, vertex_shader);
    glError;
    glAttachShader(shader_program, fragment_shader);
    glError;
    glLinkProgram(shader_program);
    uGLCheckErrorState(shader_program,
                       GL_LINK_STATUS,
                       "SHADER_PROGRAM",
                       file_name);
    assert(glIsProgram(shader_program));

    // delete shaders, return glShaderProgram identifier
    glDeleteShader(vertex_shader);
    glError;
    glDeleteShader(fragment_shader);
    glError;
    return shader_program;
}

//  [ cfarvin::TODO ] add glIsShader/Program checks
GLuint
uGLCreateShaderProgram_vgf_API(const GLchar** vertex_shader_source,
                               const GLchar** geometry_shader_source,
                               const GLchar** fragment_shader_source,
                               const char*    file_name)
{
    glError;

    // create, compile & error check vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    assert(vertex_shader);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glError;
    glCompileShader(vertex_shader);
    uGLCheckErrorState(vertex_shader,
                       GL_COMPILE_STATUS,
                       "VERTEX_SHADER",
                       file_name);

    // create, compile & error check geometry shader
    GLuint geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
    assert(geometry_shader);
    glShaderSource(geometry_shader, 1, geometry_shader_source, NULL);
    glError;
    glCompileShader(geometry_shader);
    uGLCheckErrorState(geometry_shader,
                       GL_COMPILE_STATUS,
                       "GEOMETRY_SHADER",
                       file_name);

    // create, compile & error check fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    assert(fragment_shader);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glError;
    glCompileShader(fragment_shader);
    uGLCheckErrorState(fragment_shader,
                       GL_COMPILE_STATUS,
                       "FRAGMENT_SHADER",
                       file_name);

    // create, attach vert/frag, link & error check shader program
    GLuint shader_program = glCreateProgram();
    assert(shader_program);
    glAttachShader(shader_program, vertex_shader);
    glError;
    glAttachShader(shader_program, geometry_shader);
    glError;
    glAttachShader(shader_program, fragment_shader);
    glError;
    glLinkProgram(shader_program);
    uGLCheckErrorState(shader_program,
                       GL_LINK_STATUS,
                       "SHADER_PROGRAM",
                       file_name);

    // delete shaders, return glShaderProgram identifier
    glDeleteShader(vertex_shader);
    glError;
    glDeleteShader(geometry_shader);
    glError;
    glDeleteShader(fragment_shader);
    glError;
    return shader_program;
}
