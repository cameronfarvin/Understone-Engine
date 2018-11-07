#include <engine_tools/ogl_tools.h>

void
glError()
{                                                              
    switch(glGetError())                                                
    {                                                                   
        case GL_NO_ERROR: { break; }                                    
        case GL_INVALID_ENUM:                                           
        {                                                               
            printf("[ GLERROR ] INVALID_ENUMn");
            assert(0);            
        }                                                               
        case GL_INVALID_VALUE:                                          
        {                                                               
            printf("[ GLERROR ] INVALID_VALUEn");
            assert(0);            
        }                                                               
        case GL_INVALID_OPERATION:                                      
        {                                                               
            printf("[ GLERROR ] INVALID_OPERATIONn");
            assert(0);            
        }                                                               
        case GL_INVALID_FRAMEBUFFER_OPERATION:                          
        {                                                               
            printf("[ GLERROR ] INVALID_FRAMEBUFFER_OPERATIONn");
            assert(0);            
        }                                                               
        case GL_OUT_OF_MEMORY:                                          
        {                                                               
            printf("[ GLERROR ] OUT_OF_MEMORYn");
            assert(0);
        }                                                               
        default: { assert(0); }                                         
    }
}

char stringified_pipeline_state[STRINGIFIED_PIPELINE_STATE_LENGTH];
char*
StringifyPipelineState(PipelineState pipeline_state)
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
        {            strcpy(stringified_pipeline_state, "NONE");
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
checkErrorState(GLuint object, GLenum parameter_to_check, PipelineState pipeline_state)
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
                       StringifyPipelineState(pipeline_state),
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
                       StringifyPipelineState(pipeline_state),
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
createShaderProgram_vf(const GLchar** vertex_shader_source,
                       const GLchar** fragment_shader_source)
{
    // create, compile & error check vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    assert(vertex_shader);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    checkErrorState(vertex_shader,
                    GL_COMPILE_STATUS,
                    pipeline_state_vertex_shader);

    // create, compile & error check fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    assert(fragment_shader);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    checkErrorState(fragment_shader,
                    GL_COMPILE_STATUS,
                    pipeline_state_fragment_shader);

    // create, attach vert/frag, link & error check shader program
    GLuint shader_program = glCreateProgram();
    assert(shader_program);
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    checkErrorState(shader_program,
                    GL_LINK_STATUS,
                    pipeline_state_shader_program);

    // delete shaders, return glShaderProgram identifier
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

GLuint
createShaderProgram_vgf(const GLchar** vertex_shader_source,
                        const GLchar** geometry_shader_source,
                        const GLchar** fragment_shader_source)
{
    // create, compile & error check vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    assert(vertex_shader);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    checkErrorState(vertex_shader,
                    GL_COMPILE_STATUS,
                    pipeline_state_vertex_shader);

    // create, compile & error check geometry shader
    GLuint geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
    assert(geometry_shader);
    glShaderSource(geometry_shader, 1, geometry_shader_source, NULL);
    glCompileShader(geometry_shader);
    checkErrorState(geometry_shader,
                    GL_COMPILE_STATUS,
                    pipeline_state_geometry_shader);

    // create, compile & error check fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    assert(fragment_shader);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    checkErrorState(fragment_shader,
                    GL_COMPILE_STATUS,
                    pipeline_state_fragment_shader);

    // create, attach vert/frag, link & error check shader program
    GLuint shader_program = glCreateProgram();
    assert(shader_program);
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, geometry_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    checkErrorState(shader_program,
                    GL_LINK_STATUS,
                    pipeline_state_shader_program);

    // delete shaders, return glShaderProgram identifier
    glDeleteShader(vertex_shader);
    glDeleteShader(geometry_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

