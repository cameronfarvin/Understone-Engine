#ifndef __ogl_tools__
#define __ogl_tools__ 1

#ifndef __uDEBUG_GL__
#define __uDEBUG_GL__ 1
#endif // CHECK_Gl_ERRORS

#if __linux__
#include <nix/nix_platform.h>
#elif _WIN32
#include <win/win_platform.h>
#endif // __linux__ _WIN32

#include <assert.h>
#include <math.h>
#include <string.h>
#include <data_structures/uDynamicArray.h>

#ifndef GLSL
#define GLSL(version, shaderSrc) "#version " #version "\n" #shaderSrc
#endif // GLSL

#ifndef STRINGIFIED_PIPELINE_STATE_LENGTH
#define STRINGIFIED_PIPELINE_STATE_LENGTH 18
#endif // STRINGIFIED_PIPELINE_STATE_LENGTH

#if __uDEBUG_GL__
#ifndef glError
#define glError glErrorFileLine(__FILE__, __LINE__, __func__)
#endif // glError
#else
#ifndef glError
#define glError /* GL error checking disabled. Define GL_CHECK_ERRORS to enable */
#endif // glError
#endif // __uDEBUG_GL__


#define renderSelf(x) renderSelf_API((uGLRenderTarget*)(x));
typedef struct
{
    // void* cast to (uGLRenderTarget)
    void (*renderSelf_API)(void*);

    // void* cast to (uGLRenderTarget)
    void* self;
} uGLRenderTarget;

// [ cfarvin::TODO ] Use for other platforms (don't forget)
typedef struct
{
    u8 vmajor;
    u8 vminor;
} OPENGL_STATS;
OPENGL_STATS ogl_stats;

typedef struct
{
    u16 width;
    u16 height;
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
PFNGLUNIFORM1IPROC               glUniform1i;
PFNGLGENVERTEXARRAYSPROC         glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC         glBindVertexArray;
PFNGLGENBUFFERSPROC              glGenBuffers;
PFNGLBINDBUFFERPROC              glBindBuffer;
PFNGLBUFFERDATAPROC              glBufferData;
PFNGLVERTEXATTRIBPOINTERPROC     glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLBINDFRAMEBUFFERPROC         glBindFramebuffer;
PFNGLGENFRAMEBUFFERSPROC         glGenFramebuffers;
PFNGLFRAMEBUFFERTEXTURE2DPROC    glFramebufferTexture2D;
PFNGLCHECKFRAMEBUFFERSTATUSPROC  glCheckFramebufferStatus;
PFNGLUNIFORMMATRIX4FVPROC        glUniformMatrix4fv;
PFNGLGENERATEMIPMAPPROC          glGenerateMipmap;
PFNGLISSHADERPROC                glIsShader;
PFNGLISPROGRAMPROC               glIsProgram;
PFNGLGETSTRINGIPROC              glGetStringi;
#ifdef _WIN32
PFNGLACTIVETEXTUREARBPROC        glActiveTexture;
#endif // _Win32

void
glErrorFileLine(const char* file_name,
                int line_number,
                const char* function_name);

bool
uIsExtensionSupported(uDynamicArray* uDA, const char* extension_name);


void
uGLCheckErrorState(GLuint object,
                   GLenum parameter_to_check,
                   const char* pipeline_state,
                   const char* file_name);


#define uGLCreateShaderProgram_vf(shdrSrc, frgSrc)              \
    uGLCreateShaderProgram_vf_API(shdrSrc, frgSrc, __FILE__)
GLuint
uGLCreateShaderProgram_vf_API(const GLchar** vertex_shader_source,
                              const GLchar** fragment_shader_source,
                              const char*    file_name);


#define uGLCreateShaderProgram_vgf(shdrSrc, frgSrc)             \
    uGLCreateShaderProgram_vgf_API(shdrSrc, frgSrc, __FILE__)
GLuint
uGLCreateShaderProgram_vgf_API(const GLchar** vertex_shader_source,
                               const GLchar** geometry_shader_source,
                               const GLchar** fragment_shader_source,
                               const char*    file_name);


size_t
uGLInitRenderTarget(void (*init_function)(size_t),
                    void (*render_function)(size_t));


static inline void
uGLDestroyRenderTarget(uGLRenderTarget* const render_target)
{
    if (render_target)
    {
        free(render_target);
    }
}


// [ cfarvin::TODO ] Add API && #define to autofill size_t num_targets
//
// [ cfarvin::NOTE ] Aim is to be able to pass multiple calls of this function to different
// threads wiritng to framebuffers. render_target_array should be tightly packed. Render calls
// can be made with GL state set ahead of time, rather than each render function setting up and
// tearing down the same or slightly different state via glEnable() calls and the like. The cost
// of this will be the poitner dereference and passage of pointer parameter for renderSelf();
static inline void
uGLRender(uGLRenderTarget* render_target_array, size_t num_render_targets)
{
    for (size_t ii = 0; ii < num_render_targets; ii++)
    {
            render_target_array[ii].renderSelf(render_target_array[ii].self);
    }
}


#endif // __ogl_tools__
