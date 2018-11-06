#include <nix_platform.h>

// from Khronos Example
static bool
isExtensionSupported(const char *extList, const char *extension)
{
    const char *start;
    const char *where, *terminator;

    /* Extension names should not have spaces. */
    where = strchr(extension, ' ');
    if (where || *extension == '\0')
	return false;

    for (start=extList;;) {
	where = strstr(start, extension);

	if (!where) {
            break;
	}

	terminator = where + strlen(extension);

	if ( where == start || *(where - 1) == ' ' ) {
            if ( *terminator == ' ' || *terminator == '\0' ) {
                return true;
            }
	}

	start = terminator;
    }

    return false;
}

void
x11_createWindow()
{
    //
    // Build x11 struct
    //
    /* take default DISPLAY env name as display name */
    x11.display = XOpenDisplay(NULL);
    assert(x11.display);

    x11.default_screen = XDefaultScreenOfDisplay(x11.display);
    assert(x11.default_screen);
    
    x11.default_screen_number = XDefaultScreen(x11.display);
    /* x11.connection_number     = XConnectionNumber(x11.display); */
    /* x11.default_color_map     = XDefaultColormap(x11.display, 0); */
    /* x11.graphics_context      = XDefaultGC(x11.display, 0); */
    x11.display_height        = XDisplayHeight(x11.display, x11.default_screen_number);
    x11.display_width         = XDisplayWidth(x11.display, x11.default_screen_number);

    //
    // Query GLX information (sperate from GL information)
    //
    GLint glx_vmajor;
    GLint glx_vminor;
    glXQueryVersion(x11.display, &glx_vmajor, &glx_vminor);
    assert(glx_vmajor >= 1);
    assert(glx_vminor >= 4);

    // Describe desired frame buffer
    // [ cfarvin::NOTE ] may need to render to pixel buffers for { GLX_DRAWABLE_TYPE }
    // rather than window buffers { GLX_WINDOW_BIT }. If so, will need to substitue
    // XCreateWindow() for XCreatePixmap() 
    GLint glxAttributes[] =
        {
            GLX_X_RENDERABLE    , True,
            GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
            GLX_RENDER_TYPE     , GLX_RGBA_BIT,
            GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
            GLX_RED_SIZE        , 8,
            GLX_GREEN_SIZE      , 8,
            GLX_BLUE_SIZE       , 8,
            GLX_ALPHA_SIZE      , 8,
            GLX_DEPTH_SIZE      , 24,
            GLX_STENCIL_SIZE    , 8,
            GLX_DOUBLEBUFFER    , True,
            None // last attribute must be 'None', signaling end
        };

    // Query systyem for available frame buffer(s) matching our description
    int num_possible_frame_buffers;
    GLXFBConfig* frame_buffer_candidates = glXChooseFBConfig(x11.display,
                                                             XDefaultScreen(x11.display),
                                                             glxAttributes,
                                                             &num_possible_frame_buffers);
    assert(num_possible_frame_buffers);
    printf("Possible Frame_Buffers: %d\n", num_possible_frame_buffers);

    // Select the best frame buffer based on number of samples
    int    highest_samples = 0;
    size_t highest_sample_index = 0;
    for (size_t frame_buffer_index = 0;
         frame_buffer_index < num_possible_frame_buffers;
         frame_buffer_index++)
    {
        XVisualInfo* visual_info =
            glXGetVisualFromFBConfig(x11.display,
                                     frame_buffer_candidates[frame_buffer_index]);
        if (visual_info)
        {
            int sample_buffers, samples;
            glXGetFBConfigAttrib(x11.display,
                                 frame_buffer_candidates[frame_buffer_index],
                                 GLX_SAMPLE_BUFFERS,
                                 &sample_buffers);
            glXGetFBConfigAttrib(x11.display,
                                 frame_buffer_candidates[frame_buffer_index],
                                 GLX_SAMPLES,
                                 &samples);
            if (sample_buffers && (samples > highest_samples))
            {
                highest_samples = samples;
                highest_sample_index = frame_buffer_index;
            }
        }
        XFree(visual_info);
    }
    if (highest_samples < 3)
    {
        printf("\n\n[ WARNING ] Highest available framebuffer has only %d available samples per pixel\n\n",
               highest_samples);
    }

    //
    // [ cfarvin::DEBUG ]
    //
    printf("Samples per pixel: %d\n", highest_samples);

    // set x11 struct to reflect chosen framebuffer
    GLXFBConfig target_frame_buffer = frame_buffer_candidates[highest_sample_index];
    x11.visual_info = glXGetVisualFromFBConfig(x11.display, target_frame_buffer);
    assert(x11.visual_info);
    assert(x11.default_screen_number == x11.visual_info->screen);
    XFree(frame_buffer_candidates);

    x11.set_window_attributes.border_pixel = XBlackPixel(x11.display,
                                                         x11.default_screen_number);
    x11.set_window_attributes.background_pixel = XBlackPixel(x11.display,
                                                             x11.default_screen_number);
    x11.set_window_attributes.override_redirect = true;
    x11.set_window_attributes.colormap =
        XCreateColormap(x11.display,
                        XRootWindow(x11.display,
                                    x11.default_screen_number),
                        x11.visual_info->visual,
                        AllocNone);
    x11.set_window_attributes.event_mask = ExposureMask;
    
    x11.engine_window =
        XCreateWindow(x11.display,                                      // display
                      XRootWindow(x11.display,
                                  x11.default_screen_number),           // parent
                      0,                                                // x
                      0,                                                // y
                      x11.display_width,                                // width
                      x11.display_height,                               // height
                      0,                                                // border_width
                      x11.visual_info->depth,                           // depth
                      InputOutput,                                      // class
                      x11.visual_info->visual,                          // visual
                      CWBackPixel|CWColormap|CWBorderPixel|CWEventMask, // valuemask
                      &x11.set_window_attributes);                      // attributes

    // aquire address of glxCreateContext
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddressARB(
        (const GLubyte*) "glXCreateContextAttribsARB");

    int context_attributes[] =
        {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
            GLX_CONTEXT_MINOR_VERSION_ARB, 0,
            GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            None
        };

    const char* glExtensions = glXQueryExtensionsString(x11.display,
                                                        x11.default_screen_number);
    
    if (isExtensionSupported(glExtensions, "GLX_ARB_create_context"))
    {
        x11.ogl_context = glXCreateContextAttribsARB(x11.display,
                                                     target_frame_buffer,
                                                     0,
                                                     true,
                                                     context_attributes);
    }
    else
    {
        printf("\n\n[ WARNING ] GLX_ARB_create_context extension not found\n");
        x11.ogl_context = glXCreateNewContext(x11.display,
                                              target_frame_buffer,
                                              GLX_RGBA_TYPE,
                                              0,
                                              true);
    }
    assert(x11.ogl_context);
    assert(glXMakeCurrent(x11.display, x11.engine_window, x11.ogl_context));
    assert(glXIsDirect(x11.display, x11.ogl_context));
    XSync(x11.display, false);

    printf("GL Vendor: %s\nGL Renderer: %s\nGL Version: %s\nGLSL: %s\n",
           glGetString(GL_VENDOR),
           glGetString(GL_RENDERER),
           glGetString(GL_VERSION),
           glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    XClearWindow(x11.display, x11.engine_window);
    XSelectInput(x11.display, x11.engine_window,
                 ExposureMask|
                 KeyPressMask|
                 ButtonPressMask|
                 ButtonReleaseMask|
                 MotionNotify);

    XMapWindow(x11.display, x11.engine_window);
    XGetWindowAttributes(x11.display,
                         x11.engine_window,
                         &x11.window_attributes);

    // for properly closing window with client messaging protocol
    atomWmDeleteWindow = XInternAtom(x11.display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(x11.display, x11.engine_window, &atomWmDeleteWindow, 1);

    //
    //
    // discover_opengl_function_pointers
    //
    //
    glGetShaderiv = (PFNGLGETSHADERIVPROC)
        glXGetProcAddressARB( (const GLubyte*) "glGetShaderiv");
    assert(glGetShaderiv);

    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)
        glXGetProcAddressARB( (const GLubyte*) "glGetShaderInfoLog");
    assert(glGetShaderInfoLog);

    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)
        glXGetProcAddressARB( (const GLubyte*) "glGetProgramiv");
    assert(glGetProgramiv);

    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)
        glXGetProcAddressARB( (const GLubyte*) "glGetProgramInfoLog");
    assert(glGetProgramInfoLog);

    glCreateShader = (PFNGLCREATESHADERPROC)
        glXGetProcAddressARB( (const GLubyte*) "glCreateShader");
    assert(glCreateShader);

    glCreateProgram = (PFNGLCREATEPROGRAMPROC)
        glXGetProcAddressARB( (const GLubyte*) "glCreateProgram");
    assert(glCreateProgram);
    
    glShaderSource = (PFNGLSHADERSOURCEPROC)
        glXGetProcAddressARB( (const GLubyte*) "glShaderSource");
    assert(glShaderSource);

    glCompileShader = (PFNGLCOMPILESHADERPROC)
        glXGetProcAddressARB( (const GLubyte*) "glCompileShader");
    assert(glCompileShader);
    
    glAttachShader = (PFNGLATTACHSHADERPROC)
        glXGetProcAddressARB( (const GLubyte*) "glAttachShader");
    assert(glAttachShader);

    glLinkProgram = (PFNGLLINKPROGRAMPROC)
        glXGetProcAddressARB( (const GLubyte*) "glLinkProgram");
    assert(glLinkProgram);

    glDeleteShader = (PFNGLDELETESHADERPROC)
        glXGetProcAddressARB( (const GLubyte*) "glDeleteShader");
    assert(glDeleteShader);

    glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)
        glXGetProcAddressARB( (const GLubyte*) "glGetAttribLocation");
    assert(glGetAttribLocation);

    glUseProgram = (PFNGLUSEPROGRAMPROC)
        glXGetProcAddressARB( (const GLubyte*) "glUseProgram");
    assert(glUseProgram);

    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)
        glXGetProcAddressARB( (const GLubyte*) "glGetUniformLocation");
    assert(glGetUniformLocation);
    
    /* // glUniform1f */
    /* glUniform1f = (PFNGLUNIFORM1FPROC) */
    /*     glXGetProcAddressARB( (const GLubyte*) "glUniform1f"); */
    /* assert(glUniform1f); */

    /* // glUniform2f */
    /* glUniform2f = (PFNGLUNIFORM2FPROC) */
    /*     glXGetProcAddressARB( (const GLubyte*) "glUniform2f"); */
    /* assert(glUniform2f); */

    glUniform3f = (PFNGLUNIFORM3FPROC)
        glXGetProcAddressARB( (const GLubyte*) "glUniform3f");
    assert(glUniform3f);

    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)
        glXGetProcAddressARB( (const GLubyte*) "glGenVertexArrays");
    assert(glGenVertexArrays);

    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)
        glXGetProcAddressARB( (const GLubyte*) "glBindVertexArray");
    assert(glBindVertexArray);

    glGenBuffers = (PFNGLGENBUFFERSPROC)
        glXGetProcAddressARB( (const GLubyte*) "glGenBuffers");
    assert(glGenBuffers);

    glBindBuffer = (PFNGLBINDBUFFERPROC)
        glXGetProcAddressARB( (const GLubyte*) "glBindBuffer");
    assert(glBindBuffer);

    glBufferData = (PFNGLBUFFERDATAPROC)
        glXGetProcAddressARB( (const GLubyte*) "glBufferData");
    assert(glBufferData);

    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)
        glXGetProcAddressARB( (const GLubyte*) "glVertexAttribPointer");
    assert(glVertexAttribPointer);

    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)
        glXGetProcAddressARB( (const GLubyte*) "glEnableVertexAttribArray");
    assert(glEnableVertexAttribArray);

    /* glDrawArrays = (PFNGLDRAWARRAYSPROC) */
    /*     glXGetProcAddressARB( (const GLubyte*) "glDrawArrays"); */
    /* assert(glDrawArrays); */

    x11_handleEvents();
}

EVENT
x11_handleEvents()
{   
    while(1)
    {
        XNextEvent(x11.display, &x11.event);
        switch(x11.event.type)
        {
            case ButtonPress:
            {
                switch(x11.event.xbutton.button)
                {
                    case 1:
                    {
                        mouse.x = x11.event.xmotion.x;
                        mouse.y = x11.event.xmotion.y;
                        mouse.mouse_left = IS_DOWN;
                        return EVENT_MOUSE_PRESS_LEFT;
                    }
                    case 2:
                    {
                        mouse.x = x11.event.xmotion.x;
                        mouse.y = x11.event.xmotion.y;
                        mouse.mouse_middle = IS_DOWN;
                        return EVENT_MOUSE_PRESS_MIDDLE;
                    }
                    case 3:
                    {
                        mouse.x = x11.event.xmotion.x;
                        mouse.y = x11.event.xmotion.y;
                        mouse.mouse_right = IS_DOWN;
                        return EVENT_MOUSE_PRESS_RIGHT;
                    }
                    case 4:
                    {
                        mouse.x = x11.event.xmotion.x;
                        mouse.y = x11.event.xmotion.y;
                        return EVENT_MOUSE_SCROLL_DOWN;
                    }
                    case 5:
                    {
                        mouse.x = x11.event.xmotion.x;
                        mouse.y = x11.event.xmotion.y;
                        return EVENT_MOUSE_SCROLL_UP;
                    }
                }
            }
            case ButtonRelease:
            {
                switch(x11.event.xbutton.button)
                {
                    case 1:
                    {
                        mouse.x = x11.event.xmotion.x;
                        mouse.y = x11.event.xmotion.y;
                        mouse.mouse_left = IS_UP;
                        return EVENT_MOUSE_RELEASE_LEFT;
                    }
                    case 2:
                    {
                        mouse.x = x11.event.xmotion.x;
                        mouse.y = x11.event.xmotion.y;
                        mouse.mouse_middle = IS_UP;
                        return EVENT_MOUSE_RELEASE_MIDDLE;
                    }
                    case 3:
                    {
                        mouse.x = x11.event.xmotion.x;
                        mouse.y = x11.event.xmotion.y;
                        mouse.mouse_right = IS_UP;
                        return EVENT_MOUSE_RELEASE_RIGHT;
                    }
                }
            }
            case Expose:
            {
                uint16 prev_window_attrib_width = x11.window_attributes.width;
                uint16 prev_window_attrib_height = x11.window_attributes.height;
                XGetWindowAttributes(x11.display,
                                     x11.engine_window,
                                     &x11.window_attributes);
                if ( (prev_window_attrib_width != x11.window_attributes.width)
                     || (prev_window_attrib_height != x11.window_attributes.height) )
                {
                    printf("[ NOTIFY ] Window dimensions have changed\n");
                }
                else
                {
                    printf("Window width: %d\nWindow height: %d\n",
                           x11.window_attributes.width,
                           x11.window_attributes.height);
                }
            }
            case ClientMessage:
            {
                if (x11.event.xclient.data.l[0] == atomWmDeleteWindow)
                {
                    printf("[ NOTIFY ] Recieved destroy signal\n");
                    return EVENT_CLOSE;
                }
            }
            default:
            {
                return EVENT_NONE;
            }
        }
    }
}

void
x11_destroy()
{
    glXDestroyContext(x11.display, x11.ogl_context);
    XFree(x11.visual_info);
    XFreeColormap(x11.display, x11.set_window_attributes.colormap);
    XDestroyWindow(x11.display, x11.engine_window);
    XCloseDisplay(x11.display);
}
