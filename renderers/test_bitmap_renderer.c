#include <engine_tools/ogl_tools.h>
#include <renderers/test_bitmap_renderer.h>

void
initRenderer_test_bitmap(uGLRenderTarget* const test_bitmap_renderer)
{
    const char* vshdr = GLSL(
        450 core,

        in vec2 vshdr_pos;

        void main()
        {
            gl_Position = vec4(vshdr_position, 0.0f, 1.0f);
        }

        );

    const char* fshdr = GLSL(
        450 core,

        uniform sampler2D fshdr_texture_2d;
        in vec2 fshdr_frag_coords;
        in vec3 fshdr_frag_color;
        out vec4 fshdr_final;

        void main()
        {
            fshdr_final = texture(fshdr_texture_2d, fshdr_frag_coords) * vec4(fshdr_frag_color, 1.0f);
        }

        );

    test_bitmap_renderer->shader_program = 0;
    test_bitmap_renderer->vshdr_position_location = -1;
    test_bitmap_renderer->fshdr_color_location = -1;
    test_bitmap_renderer->fshdr_frag_coords = -1;

    GLfloat position_vertex_data[] =
        {
            // top right
            0.5f, 0.5f,

            // top left
            -0.5f, 0.5f,

            // bottom left
            -0.5f, -0.5f,

            // bottom right
            0.5f, -0.5f
        };

    GLfloat texture_vertex_data[] =
        {
            // top right
            1.0f, 1.0f,

            // top left
            -1.0f, 1.0f,

            // bottom left
            -1.0f, -1.0f,

            // bottom right
            1.0f, -1.0f
        };

    GLuint indices[] =
        {
            0, 1, 2, // top right, top left, bottom left (ccw)
            0, 2, 3  // top right, bottom left, bottom right (ccw)
        };

    //
    // create_program
    //
    glError;
    test_bitmap_renderer->shader_program = uGLCreateShaderProgram_vf(&vshdr,
                                                                  &fshdr);
    assert(test_bitmap_renderer->shader_program);
    glUseProgram(test_bitmap_renderer->shader_program);
    glError;

    //
    // discover_attribute_locations
    //
    glError;

    test_bitmap_renderer->vshdr_position_location =
        glGetAttribLocation(test_bitmap_renderer->shader_program, "vshdr_pos");

    test_bitmap_renderer->fshdr_color_location =
        glGetAttribLocation(test_bitmap_renderer->shader_program, "fshdr_frag_color");

    test_bitmap_renderer->fshdr_frag_coords =
        glGetAttribLocation(test_bitmap_renderer->shader_program, "fshdr_frag_coords");
    glError;

    //
    // set default attributes
    //
    glUniform3f(test_bitmap_renderer->fshdr_color_location, 0.64, 0.64, 0.16);
    glError;

    assert(test_bitmap_renderer->vshdr_position_location != -1);
    assert(test_bitmap_renderer->fshdr_frag_color != -1);
    assert(test_bitmap_renderer->fshdr_frag_coords != -1);

    glError;
    glGenVertexArrays(1, &test_bitmap_renderer->vertex_array_buffer_location);
    glBindVertexArray(test_bitmap_renderer->vertex_array_buffer_location);
    glError;

    glGenBuffers(1, &test_bitmap_renderer->vertex_buffer_location);
    glBindBuffer(GL_ARRAY_BUFFER, test_bitmap_renderer->vertex_buffer_location);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(position_vertex_data),
                 position_vertex_data,
                 GL_STATIC_DRAW);
    glError;

    glGenBuffers(1, &test_bitmap_renderer->element_buffer_location);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, test_bitmap_renderer->element_buffer_location);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(indices),
                 indices,
                 GL_STATIC_DRAW);
    glError;

    glGenTextures(1, &test_bitmap_renderer->texture2d);
    glBindTexture(GL_TEXTURE_2D, test_bitmap_renderer->texture2d);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glError;

    // vertex position
    glVertexAttribPointer(test_bitmap_renderer->vshdr_position_location,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*) 0);
    glError;

    // texture vertex position
    glVertexAttribPointer(test_bitmap_renderer->fshdr_frag_coords,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*) 0);
    glError;

    uImage img;
    assert(uLoadBitmap("./assets/FLAG_B24.BMP", &img));
}

void
render_test_bitmap(uGLRenderTarget* const test_bitmap_renderer)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(test_bitmap_renderer->shader_program);
    glBindVertexArray(test_bitmap_renderer->vertex_array_buffer_location);
    glEnableVertexAttribArray(test_bitmap_renderer->vshdr_position_location);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glEnableVertexAttribArray(0);
}
