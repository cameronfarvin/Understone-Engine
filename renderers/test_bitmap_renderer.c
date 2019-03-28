#include <engine_tools/ogl_tools.h>
#include <renderers/test_bitmap_renderer.h>

/* // [ cfarvin::DEBUG ] [ cfarvin::REMOVE ] */
/* #define STB_IMAGE_IMPLEMENTATION */
/* #include <engine_tools/../test_dep/stb_image.h> */

void
initRenderer_test_bitmap(uGLRenderTarget* const test_bitmap_renderer)
{
    const char* vshdr = GLSL(
        450 core,

        in      vec2 vshdr_pos;
        in      vec2 vshdr_intermediate_frag_coords;

        out     vec2 fshdr_frag_coords;

        void main()
        {
            gl_Position = vec4(vshdr_pos, 0.0f, 1.0f);
            fshdr_frag_coords = vshdr_intermediate_frag_coords;
        }

        );

    const char* fshdr = GLSL(
        450 core,


        uniform vec3      fshdr_frag_color;
        uniform sampler2D fshdr_texture_2d;

        in      vec2      fshdr_frag_coords;

        out     vec4      fshdr_final;

        void main()
        {
            //fshdr_final = texture(fshdr_texture_2d, fshdr_frag_coords) * vec4(fshdr_frag_color, 1.0f);
            // fshdr_final = vec4(fshdr_frag_color, 1.0f);
            fshdr_final = texture(fshdr_texture_2d, fshdr_frag_coords);
        }

        );

    GLfloat position_vertex_data[] =
        {
            // // Position data
            // top right
            0.5f, 0.5f,

            // top left
            -0.5f, 0.5f,

            // bottom left
            -0.5f, -0.5f,

            // bottom right
            0.5f, -0.5f,

            // // Texture coordinates
            // top right
            1.0f, 1.0f,

            // top left
            -1.0f, 1.0f,

            // bottom left
            -1.0f, -1.0f,

            // bottom right
            1.0f, -1.0f

            /* // Position   //texture coords */
            /* +0.5f, +0.5f, 1.0f, 1.0f, */
            /* +0.5f, -0.5f, 1.0f, 0.0f, */
            /* -0.5f, -0.5f, 0.0f, 0.0f, */
            /* -0.5f, +0.5f, 0.0f, 1.0f */
        };

    GLuint indices[] =
        {
            0, 1, 2, // top right, top left, bottom left (ccw)
            0, 2, 3  // top right, bottom left, bottom right (ccw)
            /* 0, 3, 2, */
            /* 2, 1, 0 */
        };

    //
    // create_program
    //
    glError;
    test_bitmap_renderer->shader_program = 0;
    test_bitmap_renderer->shader_program = uGLCreateShaderProgram_vf(&vshdr,
                                                                  &fshdr,
                                                                  __FILE__);
    assert(test_bitmap_renderer->shader_program);
    glUseProgram(test_bitmap_renderer->shader_program);
    glError;

    //
    // discover_attribute_locations
    //
    test_bitmap_renderer->shdr_position_location = -1;
    test_bitmap_renderer->shdr_color_location = -1;
    test_bitmap_renderer->shdr_texture_coords_location = -1;

    test_bitmap_renderer->shdr_position_location =
        glGetAttribLocation(test_bitmap_renderer->shader_program, "vshdr_pos");

    test_bitmap_renderer->shdr_texture_coords_location =
        glGetAttribLocation(test_bitmap_renderer->shader_program, "vshdr_intermediate_frag_coords");
    glError;

    test_bitmap_renderer->shdr_color_location =
        glGetUniformLocation(test_bitmap_renderer->shader_program, "fshdr_frag_color");


    assert(test_bitmap_renderer->shdr_position_location != -1);
    /* assert(test_bitmap_renderer->fshdr_color_location != -1); */
    assert(test_bitmap_renderer->shdr_texture_coords_location != -1);

    //
    // set default attributes
    //
    glUniform3f(test_bitmap_renderer->shdr_color_location, 0.64f, 0.64f, 0.16f);
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

    glGenTextures(1, &test_bitmap_renderer->shdr_texture_2d_location);
    glBindTexture(GL_TEXTURE_2D, test_bitmap_renderer->shdr_texture_2d_location);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glError;

    //
    // Vertex attribute pointers
    //
    /*
      void glVertexAttribPointer( GLuint        index,
                                  GLint         size,
                                  GLenum        type,
                                  GLboolean     normalized,
                                  GLsizei       stride,
                                  const GLvoid* pointer);
     */
    /* glVertexAttribPointer(test_bitmap_renderer->vshdr_position_location, */
    /*                       2, */
    /*                       GL_FLOAT, */
    /*                       GL_FALSE, */
    /*                       2 * sizeof(GLfloat), */
    /*                       (void*) 0); */

    /* glVertexAttribPointer(test_bitmap_renderer->fshdr_texture_coords_location, */
    /*                       2, */
    /*                       GL_FLOAT, */
    /*                       GL_FALSE, */
    /*                       2* sizeof(GLfloat), */
    /*                       (void*) (2 * sizeof(GL_FLOAT))); */
    glVertexAttribPointer(test_bitmap_renderer->shdr_position_location,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*) 0);

    glVertexAttribPointer(test_bitmap_renderer->shdr_texture_coords_location,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*) (4 * sizeof(GL_FLOAT)));
    glError;

    //
    // Load bitmap
    //
    uImage img;
    assert(uLoadBitmap("./assets/FLAG_B24.BMP", &img));
    assert(img.img_pixels);


    glBindTexture(GL_TEXTURE_2D, test_bitmap_renderer->shdr_texture_2d_location);
    /*

      // function spec
      void glTexImage2D( GLenum         target,          // ex: GL_TEXTURE_2D
                         GLint          level,           // mipmap level
                         GLint          internal_format, // ex: GL_RGB
                         GLsizei        width,
                         GLsizei        height,
                         GLint          border,          // must be 0
                         GLenum         format,          // ex: GL_RGB
                         GLenum         type,            // ex: GL_UNSIGNED_BYTE
                         const GLvoid*  data

     */
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 (GLsizei) img.img_width,
                 (GLsizei) img.img_height,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 img.img_pixels);

    /* [ cfarvin::NOTE ] Remove? */
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    glError;
}

void
render_test_bitmap(uGLRenderTarget* const test_bitmap_renderer)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, test_bitmap_renderer->shdr_texture_2d_location);
    glUseProgram(test_bitmap_renderer->shader_program);
    glBindVertexArray(test_bitmap_renderer->vertex_array_buffer_location);
    glEnableVertexAttribArray(test_bitmap_renderer->shdr_position_location);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnableVertexAttribArray(0);
}
