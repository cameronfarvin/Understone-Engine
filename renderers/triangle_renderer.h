#ifndef __triangle_renderer__
#define __triangle_renderer__

#include <engine_tools/ogl_tools.h>
#include <engine_tools/image_tools.h>
#include <engine_tools/debug_tools.h>

void
initRenderer_triangle();

void
render_triangle();

uGLRenderTarget* triangle_renderer;

// [ cfarvin::TODO ]
/* void */
/* destroyRenderer_triangle(uGLRenderer* const triangle_renderer); */

#endif // __triangle_renderer
