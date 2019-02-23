#ifndef __event_tools__
#define __event_tools__ 1

#include <engine_tools/type_tools.h>

typedef enum
    {
        uEVENT_NONE,
        uEVENT_CLOSE,
        uEVENT_RESIZE,
        uEVENT_MOUSE_PRESS_LEFT,
        uEVENT_MOUSE_PRESS_RIGHT,
        uEVENT_MOUSE_PRESS_MIDDLE,
        uEVENT_MOUSE_RELEASE_LEFT,
        uEVENT_MOUSE_RELEASE_RIGHT,
        uEVENT_MOUSE_RELEASE_MIDDLE,
        uEVENT_MOUSE_SCROLL_UP,
        uEVENT_MOUSE_SCROLL_DOWN,
    } uEVENT;

// applies for both keyboard and mouse
typedef enum
    {
        IS_DOWN,
        IS_UP
    } uDEVICE_STATE;

typedef struct
{
    u16 x;
    u16 y;
    uDEVICE_STATE mouse_left;
    uDEVICE_STATE mouse_right;
    uDEVICE_STATE mouse_middle;
} uMOUSE_STATS;
uMOUSE_STATS  mouse;

typedef struct
{
    // alphabetical
    uDEVICE_STATE key_a;
    uDEVICE_STATE key_b;
    uDEVICE_STATE key_c;
    uDEVICE_STATE key_d;
    uDEVICE_STATE key_e;
    uDEVICE_STATE key_f;
    uDEVICE_STATE key_g;
    uDEVICE_STATE key_h;
    uDEVICE_STATE key_i;
    uDEVICE_STATE key_j;
    uDEVICE_STATE key_k;
    uDEVICE_STATE key_l;
    uDEVICE_STATE key_m;
    uDEVICE_STATE key_n;
    uDEVICE_STATE key_o;
    uDEVICE_STATE key_p;
    uDEVICE_STATE key_q;
    uDEVICE_STATE key_r;
    uDEVICE_STATE key_s;
    uDEVICE_STATE key_t;
    uDEVICE_STATE key_u;
    uDEVICE_STATE key_v;
    uDEVICE_STATE key_w;
    uDEVICE_STATE key_x;
    uDEVICE_STATE key_y;
    uDEVICE_STATE key_z;

    // numeric
    uDEVICE_STATE key_0;
    uDEVICE_STATE key_1;
    uDEVICE_STATE key_2;
    uDEVICE_STATE key_3;
    uDEVICE_STATE key_4;
    uDEVICE_STATE key_5;
    uDEVICE_STATE key_6;
    uDEVICE_STATE key_7;
    uDEVICE_STATE key_8;
    uDEVICE_STATE key_9;

    // arrows
    uDEVICE_STATE key_arrow_up;
    uDEVICE_STATE key_arrow_down;
    uDEVICE_STATE key_arrow_left;
    uDEVICE_STATE key_arrow_right;

    // shift
    uDEVICE_STATE key_shift_GENERAL;
    uDEVICE_STATE key_shift_left;
    uDEVICE_STATE key_shift_right;

    // control
    uDEVICE_STATE key_control_GENERAL;
    uDEVICE_STATE key_control_left;
    uDEVICE_STATE key_control_right;
} uKEYBOARD_STATS;
uKEYBOARD_STATS keyboard;

#endif // __event_tools__
