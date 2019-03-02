#ifndef __nix_platform__
#define __nix_platform__ 1

#include <X11/keysymdef.h>

#ifndef XK_LATIN1
#define XK_LATIN1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>
#include <GL/glxext.h>

#include <engine_tools/type_tools.h>
#include <engine_tools/event_tools.h>
#include <engine_tools/ogl_tools.h>

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*,
                                                     GLXFBConfig,
                                                     GLXContext,
                                                     bool,
                                                     const int*);

typedef struct
{
    Display*             display;
    Screen*              default_screen;
    int                  default_screen_number;
    Window               engine_window;
    Window               root_window;
    int                  display_height;
    int                  display_width;
    XEvent               event;
    XWindowAttributes    window_attributes;
    XSetWindowAttributes set_window_attributes;
    XVisualInfo*         visual_info;
    GLXContext           ogl_context;
} X11_INFO;
X11_INFO x11;

// needed to gracefully exit X11
Atom atomWmDeleteWindow;

void
uX11CreateWindow();

void
uX11Destroy();

uSystemEvent
uX11HandleEvents();

#endif // __nix_platform__
