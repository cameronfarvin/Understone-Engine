#ifndef __ogl_tools__
#define __ogl_tools__ 1

#ifndef CHECK_GL_ERRORS
#define CHECK_GL_ERRORS 1
#endif // CHECK_Gl_ERRORS

#if __linux__
#include <nix/nix_platform.h>
#elif _WIN32
#include <win/win_platform.h>
#endif // __linux__ _WIN32

#include <assert.h>
#include <math.h>
#include <string.h>
#include <uDynamicArray.h>

#ifndef GLSL
#define GLSL(version, shaderSrc) "#version " #version "\n" #shaderSrc
#endif // GLSL

#ifndef STRINGIFIED_PIPELINE_STATE_LENGTH
#define STRINGIFIED_PIPELINE_STATE_LENGTH 18
#endif // STRINGIFIED_PIPELINE_STATE_LENGTH

#if CHECK_GL_ERRORS
#ifndef glError
#define glError glErrorFileLine(__FILE__, __LINE__, __func__)
#endif // glError
#else
#ifndef glError
#define glError /* GL error checking disabled. Define GL_CHECK_ERRORS to enable */
#endif // glError
#endif // CHECK_GL_ERRORS


typedef struct
{
    GLuint texture_name;
    GLuint shader_program;

    // attribute locations
    GLint  shdr_position_location;
    GLint  shdr_modelview_mat_location;
    GLint  shdr_proj_matrix_location;
    GLint  shdr_color_location;
    GLint  shdr_texture_coords_location;
    GLuint shdr_texture_2d_location;

    // matrix data
    GLfloat modelview_matrix[16];
    GLfloat project_matrix[16];

    // vao/vbo/ebo
    GLuint vertex_array_buffer_location;
    GLuint vertex_buffer_location;
    GLuint element_buffer_location;
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

#ifndef _WIN32
PFNGLGETINTEGERVPROC             glGetIntegerv;
#endif

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
PFNGLUNIFORMMATRIX4FVPROC        glUniformMatrix4fv;
PFNGLGENERATEMIPMAPPROC          glGenerateMipmap;
PFNGLUNIFORM1IPROC               glUniform1i;
PFNGLISSHADERPROC                glIsShader;
PFNGLISPROGRAMPROC               glIsProgram;
PFNGLGETSTRINGIPROC              glGetStringi;
PFNGLACTIVETEXTUREARBPROC        glActiveTextureARB;


//
// signatures
//
void
uGLCheckErrorState(GLuint object,
                   GLenum parameter_to_check,
                   const char* pipeline_state,
                   const char* file_name);

GLuint
uGLCreateShaderProgram_vf(const GLchar** vertex_shader_source,
                          const GLchar** fragment_shader_source,
                          const char*    file_name);

GLuint
uGLCreateShaderProgram_vgf(const GLchar** vertex_shader_source,
                           const GLchar** geometry_shader_source,
                           const GLchar** fragment_shader_source,
                           const char*    file_name);
void
glErrorFileLine(const char* file_name,
                int line_number,
                const char* function_name);

// [ cfarvin::NOTE ] Remvoed this as we use uDyanicArray now. See uIsExtensionSupported()
/* extern bool */
/* isExtensionSupported(const char *extList, const char *extension); */

bool
uIsExtensionSupported(uDynamicArray* uDA, const char* extension_name);


#endif // __ogl_tools__
