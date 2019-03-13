#ifndef __test_bitmap_renderer__
#define __test_bitmap_renderer__

#include <GL/gl.h>
#include <GL/glu.h>
#include <engine_tools/ogl_tools.h>
#include <engine_tools/image_tools.h>

void
initRenderer_test_bitmap(uGLRenderTarget* const test_bitmap_renderer);

void
render_test_bitmap(uGLRenderTarget* const test_bitmap_renderer);

// [ cfarvin::TODO ]
/* void */
/* destroyRenderer_test_bitmap(uGLRenderer* const test_bitmap_renderer); */


#endif // __test_bitmap_renderer
