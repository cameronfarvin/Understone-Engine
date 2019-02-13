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
    } uINPUT_STATE;

typedef struct
{
    u16 x;
    u16 y;
    uINPUT_STATE mouse_left;
    uINPUT_STATE mouse_right;
    uINPUT_STATE mouse_middle;
} uMOUSE_STATS;
uMOUSE_STATS  mouse;

typedef struct
{
    // alphabetical
    uINPUT_STATE key_a;
    uINPUT_STATE key_b;
    uINPUT_STATE key_c;
    uINPUT_STATE key_d;
    uINPUT_STATE key_e;
    uINPUT_STATE key_f;
    uINPUT_STATE key_g;
    uINPUT_STATE key_h;
    uINPUT_STATE key_i;
    uINPUT_STATE key_j;
    uINPUT_STATE key_k;
    uINPUT_STATE key_l;
    uINPUT_STATE key_m;
    uINPUT_STATE key_n;
    uINPUT_STATE key_o;
    uINPUT_STATE key_p;
    uINPUT_STATE key_q;
    uINPUT_STATE key_r;
    uINPUT_STATE key_s;
    uINPUT_STATE key_t;
    uINPUT_STATE key_u;
    uINPUT_STATE key_v;
    uINPUT_STATE key_w;
    uINPUT_STATE key_x;
    uINPUT_STATE key_y;
    uINPUT_STATE key_z;

    // numeric
    uINPUT_STATE key_0;
    uINPUT_STATE key_1;
    uINPUT_STATE key_2;
    uINPUT_STATE key_3;
    uINPUT_STATE key_4;
    uINPUT_STATE key_5;
    uINPUT_STATE key_6;
    uINPUT_STATE key_7;
    uINPUT_STATE key_8;
    uINPUT_STATE key_9;

    // arrows
    uINPUT_STATE key_arrow_up;
    uINPUT_STATE key_arrow_down;
    uINPUT_STATE key_arrow_left;
    uINPUT_STATE key_arrow_right;

    // shift
    uINPUT_STATE key_shift_GENERAL;
    uINPUT_STATE key_shift_left;
    uINPUT_STATE key_shift_right;

    // control
    uINPUT_STATE key_control_GENERAL;
    uINPUT_STATE key_control_left;
    uINPUT_STATE key_control_right;
} uKEYBOARD_STATS;
uKEYBOARD_STATS keyboard;

#endif // __event_tools__
