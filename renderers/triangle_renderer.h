#ifndef __triangle_renderer__
#define __triangle_renderer__

#include <GL/gl.h>
#include <GL/glu.h>
#include <engine_tools/ogl_tools.h>

void
initRenderer_triangle(uGLRenderer* const triangle_renderer);

void
render_triangle(uGLRenderer* const tirangle_renderer);

// [ cfarvin::TODO ]
/* void */
/* destroyRenderer_triangle(uGLRenderer* const triangle_renderer); */

#endif // __triangle_renderer__
