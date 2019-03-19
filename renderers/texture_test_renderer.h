#ifndef __texture_test_renderer__
#define __texture_test_renderer__

#include <GL/gl.h>
#include <GL/glu.h>
#include <engine_tools/ogl_tools.h>
#include <engine_tools/image_tools.h>

void
initRenderer_texture_test(uGLRenderTarget* const texture_test_renderer);

void
render_texture_test(uGLRenderTarget* const texture_test_renderer);

#endif // __texture_test_renderer
