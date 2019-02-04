#ifndef __ogl_tools__
#define __ogl_tools__ 1

/* #include <GL/gl.h> */
/* #include <GL/glu.h> */

#if __linux__
#include <nix/nix_platform.h>
#elif _WIN32
#include <win/win_platform.h>
#endif // __linux__ _WIN32

#include <assert.h>

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

typedef struct
{
    // [ cfarvin::NOTE ] Not sure if it is necessary to keep pointers to shaders.
    // Shaders for now will be static, and it is not necessary to store the char*
    // so long as we have the gpuid of the shader program.
    /* char* const* vshdr; */
    /* char* const* fshdr; */
    GLuint shader_program;

    //
    // attribute_locations
    //
    GLint vshdr_position_location;
    GLint fshdr_color_location;

    //
    // vao_vbo
    //
    GLuint vertex_array_bufer_location;
    GLuint vertex_buffer_location;

    // [ cfarvin::NOTE ] We will absolutely want to have access to the vertex data.
    // For now, commenting out just to get things working.
    //
    // triangle_vertex_data
    //
    /* GLfloat** triangle_vertex_data; */
    
    //
    // texture map
    //
    GLuint texture_map;
    /* GLubyte** test_fbo_data; */
    GLuint depth_texture;
    GLuint framebuffer;

} uGLRenderer;

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
uGLStringifyPipelineState(PipelineState pipeline_state);

void
uGLCheckErrorState(GLuint object, GLenum parameter_to_check,
                PipelineState pipeline_state);

GLuint
uGLCreateShaderProgram_vf(const GLchar** vertex_shader_source,
                       const GLchar** fragment_shader_source);

GLuint
uGLCreateShaderProgram_vgf(const GLchar** vertex_shader_source,
                        const GLchar** geometry_shader_source,
                        const GLchar** fragment_shader_source);

#endif // __ogl_tools__
