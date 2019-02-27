#ifndef __event_tools__
#define __event_tools__ 1

#include <engine_tools/type_tools.h>

typedef enum
    {
        uEventNone,
        uEventClose,
        uEventResize,
    } uSystemEvent;

typedef struct
{
    u16 x;
    u16 y;
} uMousePos;
uMousePos  mouse_pos;

static uint64_t input_state;
#define uKEY_A             (1UL << 0)
#define uKEY_B             (1UL << 1)
#define uKEY_C             (1UL << 2)
#define uKEY_D             (1UL << 3)
#define uKEY_E             (1UL << 4)
#define uKEY_F             (1UL << 5)
#define uKEY_G             (1UL << 6)
#define uKEY_H             (1UL << 7)
#define uKEY_I             (1UL << 8)
#define uKEY_J             (1UL << 9)
#define uKEY_K             (1UL << 10)
#define uKEY_L             (1UL << 11)
#define uKEY_M             (1UL << 12)
#define uKEY_N             (1UL << 13)
#define uKEY_O             (1UL << 14)
#define uKEY_P             (1UL << 15)
#define uKEY_Q             (1UL << 16)
#define uKEY_R             (1UL << 17)
#define uKEY_S             (1UL << 18)
#define uKEY_T             (1UL << 19)
#define uKEY_U             (1UL << 20)
#define uKEY_V             (1UL << 21)
#define uKEY_W             (1UL << 22)
#define uKEY_X             (1UL << 23)
#define uKEY_Y             (1UL << 24)
#define uKEY_Z             (1UL << 25)
#define uKEY_0             (1UL << 26)
#define uKEY_1             (1UL << 27)
#define uKEY_2             (1UL << 28)
#define uKEY_3             (1UL << 29)
#define uKEY_4             (1UL << 30)
#define uKEY_5             (1UL << 31)
#define uKEY_6             (1UL << 32)
#define uKEY_7             (1UL << 33)
#define uKEY_8             (1UL << 34)
#define uKEY_9             (1UL << 35)
#define uKey_arrow_up      (1UL << 36)
#define uKey_arrow_down    (1UL << 37)
#define uKey_arrow_left    (1UL << 38)
#define uKey_arrow_right   (1UL << 39)
#define uKey_shift_left    (1UL << 40)
#define uKey_shift_right   (1UL << 41)
#define uKey_control_left  (1UL << 42)
#define uKey_control_right (1UL << 43)
#define uMouse_right       (1UL << 44)
#define uMouse_left        (1UL << 45)
#define uMouse_middle      (1UL << 46)

static inline void uSetInputPressed(uint64_t key)
{
    input_state |= key;
}

static inline void uSetInputReleased(uint64_t key)
{
    input_state &= ~key;
}

static inline uint64_t uGetInputPressed(uint64_t key)
{
    return input_state & key;
}

#endif // __event_tools__
