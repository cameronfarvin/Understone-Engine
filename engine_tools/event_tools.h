#ifndef __event_tools__
#define __event_tools__ 1

typedef enum
    {
        EVENT_NONE,
        EVENT_CLOSE,
        EVENT_RESIZE,
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

#endif // __event_tools__
