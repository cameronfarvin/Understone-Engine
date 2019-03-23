#include <engine_tools/ogl_tools.h>

void
glErrorFileLine(const char* file_name, int line_number, const char* function_name)
{
    switch(glGetError()) 
    { 
        case GL_NO_ERROR: 
            { 
                break;
            } 
        case GL_INVALID_ENUM: 
            { 
                printf("[ GLERROR ] INVALID_ENUM\nFILE: %s\nLINE: %d\nFUNCTION: %s\n",
                        file_name,
                        line_number,
                        function_name);                
                assert(0);
            } 
        case GL_INVALID_VALUE: 
            { 
                printf("[ GLERROR ] INVALID_VALUE\nFILE: %s\nLINE: %d\nFUNCTION: %s\n",
                        file_name,
                        line_number,
                        function_name);                
                assert(0);

            } 
        case GL_INVALID_OPERATION: 
            { 
                printf("[ GLERROR ] INVALID_OPERATION\nFILE: %s\nLINE: %d\nFUNCTION: %s\n",
                        file_name,
                        line_number,
                        function_name);                
                assert(0);
            }                                                               
        case GL_INVALID_FRAMEBUFFER_OPERATION:                          
            {                                                               
                printf("[ GLERROR ] INVALID_FRAMEBUFFER_OPERATION\nFILE: %s\nLINE: %d\nFUNCTION: %s\n",
                        file_name,
                        line_number,
                        function_name);                
                assert(0);
            }                                                               
        case GL_OUT_OF_MEMORY:                                          
            {                                                               
                printf("[ GLERROR ] OUT_OF_MEMORY\nFILE: %s\nLINE: %d\nFUNCTION: %s\n",
                        file_name,
                        line_number,
                        function_name);                
                assert(0);
            }                                                               
        default: { assert(0); }                                         
    }
}


char stringified_pipeline_state[STRINGIFIED_PIPELINE_STATE_LENGTH];

char*
uGLStringifyPipelineState(PipelineState pipeline_state)
{
    for (size_t string_index = 0;
            string_index < STRINGIFIED_PIPELINE_STATE_LENGTH;
            string_index++)
    {
        stringified_pipeline_state[string_index] = '\0';
    }

    switch(pipeline_state)
    {
        case pipeline_state_none:
            {
#if _WIN32
#pragma warning(push)
#pragma warning( disable: 4996 ) 
#endif // _WIN32
                strcpy(stringified_pipeline_state, "NONE");
#if _WIN32
#pragma warning(push)
#pragma warning( disable: 4996 )    
#endif // _WIN32
                break;
            }
        case pipeline_state_vertex_shader:
            {
                strcpy(stringified_pipeline_state, "VERTEX SHADER");
                break;
            }
        case pipeline_state_geometry_shader:
            {
                strcpy(stringified_pipeline_state, "GEOMETRY SHADER");
                break;
            }
        case pipeline_state_fragment_shader:
            {
                strcpy(stringified_pipeline_state, "FRAGMENT SHADER");
                break;
            }
        case pipeline_state_shader_program:
            {
                strcpy(stringified_pipeline_state, "SHADER PROGRAM");
                break;
            }
        default:
            {
                assert(0);
            }
    }

    return &stringified_pipeline_state[0];
}

void
uGLCheckErrorState(GLuint object, GLenum parameter_to_check, PipelineState pipeline_state)
{
    GLint   err = GL_TRUE;
    GLsizei log_length = 0;
    char* message = (char*) malloc(1024 * sizeof(char));

    switch (parameter_to_check)
    {
        case GL_COMPILE_STATUS:
            {
                glGetShaderiv(object, parameter_to_check, &err);
                glGetShaderInfoLog(object, 1024, &log_length, message);

                if (err != GL_TRUE)
                {
                    printf("[ ERROR | GL_COMPILE_STATUS | %s ] %s",
                            uGLStringifyPipelineState(pipeline_state),
                            message);

                    fflush(stdout);
                    if (message) { free(message); }
                    message = NULL;
                    assert(false);
                }
                break;
            }
        case GL_LINK_STATUS:
            {
                glGetProgramiv(object, parameter_to_check, &err);
                glGetProgramInfoLog(object, 1024, &log_length, message);

                // small code repetition to help see which case threw assert
                if (err != GL_TRUE)
                {
                    printf("[ ERROR | GL_LINK_STATUS | %s ] %s",
                            uGLStringifyPipelineState(pipeline_state),
                            message);

                    fflush(stdout);
                    if (message) { free(message); }
                    message = NULL;
                    assert(false);
                }
                break;
            }
        default: assert(false);
    }
    if (message) { free(message); }
}

GLuint
uGLCreateShaderProgram_vf(const GLchar** vertex_shader_source,
        const GLchar** fragment_shader_source)
{
    // create, compile & error check vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER); // [ cfarvin::NOTE ] win32 crashes here. Check GLContext creation.
    assert(vertex_shader);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    uGLCheckErrorState(vertex_shader,
            GL_COMPILE_STATUS,
            pipeline_state_vertex_shader);

    // create, compile & error check fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    assert(fragment_shader);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    uGLCheckErrorState(fragment_shader,
            GL_COMPILE_STATUS,
            pipeline_state_fragment_shader);

    // create, attach vert/frag, link & error check shader program
    GLuint shader_program = glCreateProgram();
    assert(shader_program);
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    uGLCheckErrorState(shader_program,
            GL_LINK_STATUS,
            pipeline_state_shader_program);

    // delete shaders, return glShaderProgram identifier
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

GLuint
uGLCreateShaderProgram_vgf(const GLchar** vertex_shader_source,
        const GLchar** geometry_shader_source,
        const GLchar** fragment_shader_source)
{
    // create, compile & error check vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    assert(vertex_shader);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    uGLCheckErrorState(vertex_shader,
            GL_COMPILE_STATUS,
            pipeline_state_vertex_shader);

    // create, compile & error check geometry shader
    GLuint geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
    assert(geometry_shader);
    glShaderSource(geometry_shader, 1, geometry_shader_source, NULL);
    glCompileShader(geometry_shader);
    uGLCheckErrorState(geometry_shader,
            GL_COMPILE_STATUS,
            pipeline_state_geometry_shader);

    // create, compile & error check fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    assert(fragment_shader);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    uGLCheckErrorState(fragment_shader,
            GL_COMPILE_STATUS,
            pipeline_state_fragment_shader);

    // create, attach vert/frag, link & error check shader program
    GLuint shader_program = glCreateProgram();
    assert(shader_program);
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, geometry_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    uGLCheckErrorState(shader_program,
            GL_LINK_STATUS,
            pipeline_state_shader_program);

    // delete shaders, return glShaderProgram identifier
    glDeleteShader(vertex_shader);
    glDeleteShader(geometry_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

