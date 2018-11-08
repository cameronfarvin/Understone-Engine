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

#ifndef glError
#define print_assert(msg) printf(msg); assert(0);
#define glError                                                         \
    switch(glGetError())                                                \
    {                                                                   \
        case GL_NO_ERROR: { break; }                                    \
        case GL_INVALID_ENUM:                                           \
        {                                                               \
            print_assert("[ GLERROR ] INVALID_ENUM\n");                 \
        }                                                               \
        case GL_INVALID_VALUE:                                          \
        {                                                               \
            print_assert("[ GLERROR ] INVALID_VALUE\n");                \
        }                                                               \
        case GL_INVALID_OPERATION:                                      \
        {                                                               \
            print_assert("[ GLERROR ] INVALID_OPERATION\n");            \
        }                                                               \
        case GL_INVALID_FRAMEBUFFER_OPERATION:                          \
        {                                                               \
            print_assert("[ GLERROR ] INVALID_FRAMEBUFFER_OPERATION\n"); \
        }                                                               \
        case GL_OUT_OF_MEMORY:                                          \
        {                                                               \
            print_assert("[ GLERROR ] OUT_OF_MEMORY\n");                \
        }                                                               \
        default: { assert(0); }                                         \
    }
#endif // glError

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
PFNGLUNIFORM1FPROC               glUniform1f;
PFNGLUNIFORM2FPROC               glUniform2f;
PFNGLUNIFORM3FPROC               glUniform3f;
PFNGLGENVERTEXARRAYSPROC         glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC         glBindVertexArray;
PFNGLGENBUFFERSPROC              glGenBuffers;
PFNGLBINDBUFFERPROC              glBindBuffer;
PFNGLBUFFERDATAPROC              glBufferData;
PFNGLVERTEXATTRIBPOINTERPROC     glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
/* PFNGLDRAWARRAYSPROC              glDrawArrays; */
PFNGLBINDFRAMEBUFFERPROC         glBindFramebuffer;
PFNGLGENFRAMEBUFFERSPROC         glGenFramebuffers;
/* PFNGLGENTEXTURESPROC             glGenTextures; */
/* PFNGLBINDTEXTURESPROC             glBindTexture; // NOTE: glBindTexture(s), not glBindTexture in PFNPROC */
/* PFNGLTEXPARAMETERIPROC           glTexParameteri; */
/* PFNGLTEXIMAGE2DPROC              glTexImage2D; */
PFNGLFRAMEBUFFERTEXTURE2DPROC    glFramebufferTexture2D;
PFNGLCHECKFRAMEBUFFERSTATUSPROC  glCheckFramebufferStatus;

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

#endif // __ogl_tools__
