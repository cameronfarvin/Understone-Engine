#include <nix_platform.h>
#include <win_platform.h>

#include <engine_tools.h>

#include <stdint.h>

#ifndef STRINGIFIED_PIPELINE_STATE_LENGTH
#define STRINGIFIED_PIPELINE_STATE_LENGTH 18
#endif // STRINGIFIED_PIPELINE_STATE_LENGTH
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

static uint8 RUNNING = 1;

void cleanEngineAllocations()
{
}

void refreshInputState()
{
#if __linux__
    EVENT event = x11_handleEvents();
#endif // __linux__
        
    stats.events_handled_this_loop = 0;
                    
    switch(event)
    {
        case EVENT_NONE:
        {
            break;
        }
        case EVENT_CLOSE:
        {
            RUNNING = 0;
            stats.events_handled_this_loop++;
            break;
        }
        case EVENT_MOUSE_PRESS_LEFT:
        {
            printf("ButtonPress | Mouse Left\n");
            stats.events_handled_this_loop++;
            break;
        }
        case EVENT_MOUSE_PRESS_RIGHT:
        {
            printf("ButtonPress | Mouse Right\n");
            stats.events_handled_this_loop++;
            break;
        }
        case EVENT_MOUSE_PRESS_MIDDLE:
        {
            printf("ButtonPress | Mouse Middle\n");
            stats.events_handled_this_loop++;
            break;
        }
        case EVENT_MOUSE_RELEASE_LEFT:
        {
            printf("ButtonRelease | Mouse Left\n");
            stats.events_handled_this_loop++;
            break;
        }
        case EVENT_MOUSE_RELEASE_RIGHT:
        {
            printf("ButtonRelease | Mouse Right\n");
            stats.events_handled_this_loop++;
            break;
        }
        case EVENT_MOUSE_RELEASE_MIDDLE:
        {
            printf("ButtonRelease | Mouse Middle\n");
            stats.events_handled_this_loop++;
            break;
        }
        case EVENT_MOUSE_SCROLL_UP:
        {
            printf("ButtonPress | Mouse Scroll Up\n");
            stats.events_handled_this_loop++;                    
            break;
        }
        case EVENT_MOUSE_SCROLL_DOWN:
        {
            printf("ButtonPress | Mouse Scroll Down\n");
            stats.events_handled_this_loop++;
            break;
        }
    }
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
}


int main(int argc, char** argv)
{
    for (size_t cl_arg = 0; cl_arg < (size_t) argc; cl_arg++)
    {
        printf("cl_arg[%zd]: %s\n", cl_arg, argv[cl_arg]);
    }
    
#if __linux__
    x11_createWindow();
#endif // __linux__

    glClearColor(0.5f, 0.05f, 0.73f, 1.0f);

    const  char* vshdr = GLSL(441 core,

                              in  vec2 vshdr_pos;
                              out vec3 vshdr_color_out;
         
                              main()
                              {
                                  gl_Position(fhdr_pos.x, vshdr_pos.y, 0.0f, 1.0f);
                                  vshdr_color_out = vec4(vshdr_color_out.x,
                                                         vshdr_color_out.y,
                                                         vshdr_color_out.z,
                                                         1.0f);
                              });

    const fshdr = GLSL(441 core,
                       
        );
    

    while(RUNNING)
    {
        refreshInputState();
        render();


#if __linux__
        glXSwapBuffers(x11.display, x11.engine_window);
#endif // __linux__
    }

#if __linux__
    x11_destroy();
#endif // __linux__

    cleanEngineAllocations();
    
    return 0;
}
