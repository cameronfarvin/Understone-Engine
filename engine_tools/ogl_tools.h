#ifndef __ogl_tools__
#define __ogl_tools__ 1

#include <GL/gl.h>
#include <GL/glu.h>

#include <nix/nix_platform.h>
#include <win/win_platform.h>

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif

#ifndef GLSL
#define GLSL(version, shaderSrc) "#version " #version "\n" #shaderSrc
#endif // GLSL

#ifndef STRINGIFIED_PIPELINE_STATE_LENGTH
#define STRINGIFIED_PIPELINE_STATE_LENGTH 18
#endif // STRINGIFIED_PIPELINE_STATE_LENGTH

typedef enum
    {
        pipeline_state_none,
        pipeline_state_vertex_shader,
        pipeline_state_geometry_shader,
        pipeline_state_fragment_shader,
        pipeline_state_shader_program,
    } PipelineState;

// [ cfarvin::TODO ] Use for other platforms (don't forget)
typedef struct
{
    uint8 vmajor;
    uint8 vminor;
} OPENGL_STATS;
OPENGL_STATS ogl_stats;


typedef struct
{
    uint16_t width;
    uint16_t height;
} VIEWPORT;
VIEWPORT viewport;


PFNGLGETSHADERIVPROC             glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC        glGetShaderInfoLog;
PFNGLGETPROGRAMIVPROC            glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC       glGetProgramInfoLog;
PFNGLCREATESHADERPROC            glCreateShader;
PFNGLCREATEPROGRAMPROC           glCreateProgram;
PFNGLSHADERSOURCEPROC            glShaderSource;
PFNGLCOMPILESHADERPROC           glCompileShader;
PFNGLATTACHSHADERPROC            glAttachShader;
PFNGLLINKPROGRAMPROC             glLinkProgram;
PFNGLDELETESHADERPROC            glDeleteShader;
PFNGLUSEPROGRAMPROC              glUseProgram;
PFNGLGETATTRIBLOCATIONPROC       glGetAttribLocation;
PFNGLGETUNIFORMLOCATIONPROC      glGetUniformLocation;
/* PFNGLUNIFORM1FPROC               glUniform1f; */
/* PFNGLUNIFORM2FPROC               glUniform2f; */
PFNGLUNIFORM3FPROC               glUniform3f;
PFNGLGENVERTEXARRAYSPROC         glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC         glBindVertexArray;
PFNGLGENBUFFERSPROC              glGenBuffers;
PFNGLBINDBUFFERPROC              glBindBuffer;
PFNGLBUFFERDATAPROC              glBufferData;
PFNGLVERTEXATTRIBPOINTERPROC     glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
/* PFNGLDRAWARRAYSPROC              glDrawArrays; */

//
// signatures
//
char*
StringifyPipelineState(PipelineState pipeline_state);

void
checkErrorState(GLuint object, GLenum parameter_to_check,
                PipelineState pipeline_state);

GLuint
createShaderProgram_vf(const GLchar** vertex_shader_source,
                       const GLchar** fragment_shader_source);

GLuint
createShaderProgram_vgf(const GLchar** vertex_shader_source,
                        const GLchar** geometry_shader_source,
                        const GLchar** fragment_shader_source);

void
glError();

#endif // __ogl_tools__
