//
// This file assumes platform layer has included OpenGL types, enums, & strcts
// prior to it's inclusion. Typedefs for function pointers are defined here, but
// should be populated in the platform layer on window/context construction.
//
#ifndef __engine_tools__
#define __engine_tools__

#include <stdint.h>
#include <stddef.h>

#define int8    int8_t
#define int16   int16_t
#define int32   int32_t
#define int64   int64_t
#define uint8   uint8_t
#define uint16  uint16_t
#define uint32  uint32_t
#define uint64  uint64_t
#define float32 float
#define float64 double

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif

PFNGLGETSHADERIVPROC       glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC  glGetShaderInfoLog;
PFNGLGETPROGRAMIVPROC      glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;

#ifndef GLSL
#define GLSL(version, shaderSrc) "#version " #version "\n" #shaderSrc
#endif // GLSL

#ifndef glError
#define print_assert(msg) FLUSH_PRINT(msg); assert(0)
#define glError()                                                       \
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

typedef enum
    {
        false = 0,
        true = 1
    } bool;

// [ cfarvin::TODO ] Use for other platforms (don't forget)
typedef struct
{
    uint8 vmajor;
    uint8 vminor;
} OPENGL_STATS;
OPENGL_STATS ogl_stats;

typedef enum
    {
        EVENT_NONE,
        EVENT_CLOSE,
        EVENT_MOUSE_PRESS_LEFT,
        EVENT_MOUSE_PRESS_RIGHT,
        EVENT_MOUSE_PRESS_MIDDLE,
        EVENT_MOUSE_RELEASE_LEFT,
        EVENT_MOUSE_RELEASE_RIGHT,
        EVENT_MOUSE_RELEASE_MIDDLE,
        EVENT_MOUSE_SCROLL_UP,
        EVENT_MOUSE_SCROLL_DOWN,
    } EVENT;

// applies for both keyboard and mouse
typedef enum
    {
        IS_DOWN,
        IS_UP
    }INPUT_STATE;

typedef struct
{
    uint16 x;
    uint16 y;
    INPUT_STATE mouse_left;
    INPUT_STATE mouse_right;
    INPUT_STATE mouse_middle;
} MOUSE_STATS;
MOUSE_STATS  mouse;

typedef struct
{
    // alphabetical
    INPUT_STATE key_a;
    INPUT_STATE key_b;
    INPUT_STATE key_c;
    INPUT_STATE key_d;
    INPUT_STATE key_e;
    INPUT_STATE key_f;
    INPUT_STATE key_g;
    INPUT_STATE key_h;
    INPUT_STATE key_i;
    INPUT_STATE key_j;
    INPUT_STATE key_k;
    INPUT_STATE key_l;
    INPUT_STATE key_m;
    INPUT_STATE key_n;
    INPUT_STATE key_o;
    INPUT_STATE key_p;
    INPUT_STATE key_q;
    INPUT_STATE key_r;
    INPUT_STATE key_s;
    INPUT_STATE key_t;
    INPUT_STATE key_u;
    INPUT_STATE key_v;
    INPUT_STATE key_w;
    INPUT_STATE key_x;
    INPUT_STATE key_y;
    INPUT_STATE key_z;
    
    // numeric
    INPUT_STATE key_0;
    INPUT_STATE key_1;
    INPUT_STATE key_2;
    INPUT_STATE key_3;
    INPUT_STATE key_4;
    INPUT_STATE key_5;
    INPUT_STATE key_6;
    INPUT_STATE key_7;
    INPUT_STATE key_8;
    INPUT_STATE key_9;
    
    // arrows
    INPUT_STATE key_arrow_up;
    INPUT_STATE key_arrow_down;
    INPUT_STATE key_arrow_left;
    INPUT_STATE key_arrow_right;

    // shift
    INPUT_STATE key_shift_GENERAL;
    INPUT_STATE key_shift_left;
    INPUT_STATE key_shift_right;

    // control
    INPUT_STATE key_control_GENERAL;
    INPUT_STATE key_control_left;
    INPUT_STATE key_control_right;
} KEYBOARD_STATS;
KEYBOARD_STATS keyboard;

typedef struct
{
    uint8 events_handled_this_loop;
} STATISTICS;
STATISTICS stats;

typedef struct
{
    size_t height;
    size_t width;
} HEIGHT_WIDTH;

#endif // __engine_tools__
