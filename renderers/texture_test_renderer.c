#include <renderers/texture_test_renderer.h>

// [ cfarvin::DEBUG ] [ cfarvin::REMOVE ]
#define STB_IMAGE_IMPLEMENTATION
#include <engine_tools/../test_dep/stb_image.h>


void
initRenderer_texture_test()
{
    const char* vshdr = GLSL
        (450 core,

         in      vec2 vshdr_pos;
         in      vec2 texture_coordinates_in;

         out     vec2 texture_coordinates;

         void main()
         {
             gl_Position = vec4(vshdr_pos,
                                0.0f,
                                1.0f);

             texture_coordinates = texture_coordinates_in;
         });

    const char* fshdr = GLSL
        (450 core,

         uniform sampler2D texture2d;

         in      vec2      texture_coordinates;

         out     vec4      fshdr_final;

         void main()
         {
             fshdr_final = texture(texture2d, texture_coordinates);
         });

    if (!texture_test_renderer)
    {
        texture_test_renderer = (uGLRenderTarget*)malloc(sizeof(uGLRenderTarget));
    }

    texture_test_renderer->shader_program = 0;
    texture_test_renderer->shdr_position_location = -1;
    texture_test_renderer->shdr_texture_coords_location = -1;

    // [ cfarvin::RETURN ]
    // Thinking about int assignments on mem vs gc for textures, what they mean.

    GLfloat texture_test_vertex_data[] =
        {
            // vertex coords
            // top right [0]
            0.25, 0.25,

            // top left [1]
            -0.25, 0.25,

            // bottom left [2]
            -0.25, -0.25,

            // bottom right [3]
            0.25, -0.25,

            // texture coords
            // top right [0]
            1.0, 1.0,

            // top left [1]
            0.0, 1.0,

            // bottom left [2]
            0.0, 0.0,

            // bottom right [3]
            1.0, 0.0,
        };

    GLint ebo_indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };

    //
    // create program
    //
    glError;
    texture_test_renderer->shader_program = uGLCreateShaderProgram_vf(&vshdr,
                                                                      &fshdr);
    glError;
    assert(texture_test_renderer->shader_program);
    glUseProgram(texture_test_renderer->shader_program);
    glError;

    //
    // discover attribute location
    //
    texture_test_renderer->shdr_position_location =
        glGetAttribLocation(texture_test_renderer->shader_program, "vshdr_pos");
    assert(texture_test_renderer->shdr_position_location != -1);
    glError;

    texture_test_renderer->shdr_texture_coords_location =
        glGetAttribLocation(texture_test_renderer->shader_program, "texture_coordinates_in");;
    assert(texture_test_renderer->shdr_texture_coords_location != -1);
    glError;

    //
    // populate uniforms
    //
    glUniform1i(glGetUniformLocation(texture_test_renderer->shader_program, "texture2d"), 0);
    glError;

    //
    // generate/buffer arrays
    //
    glGenVertexArrays(1, &texture_test_renderer->vertex_array_object);
    glBindVertexArray(texture_test_renderer->vertex_array_object);
    glError;

    glGenBuffers(1, &texture_test_renderer->element_buffer_object);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texture_test_renderer->element_buffer_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(ebo_indices),
                 ebo_indices,
                 GL_STATIC_DRAW);
    glError;

    glGenBuffers(1, &texture_test_renderer->vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, texture_test_renderer->vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(texture_test_vertex_data),
                 texture_test_vertex_data,
                 GL_STATIC_DRAW);
    glError;

    //
    // generate textures
    //
    glGenTextures(1, &texture_test_renderer->texture_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_test_renderer->texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glError;

    /*
    // function spec
    void glVertexAttribPointer(
    GLuint        index,
    GLint         size,
    GLenum        type,
    GLboolean     normalized,
    GLsizei       stride,
    const GLvoid* pointer offset
    );
    */

    glError;
    glVertexAttribPointer(texture_test_renderer->shdr_position_location, // index
                          2, // size
                          GL_FLOAT, // type
                          GL_FALSE, // normalized
                          0, // stride
                          (void*) 0); // pointer offset

    glVertexAttribPointer(texture_test_renderer->shdr_texture_coords_location, // index
                          2, // size
                          GL_FLOAT, // type
                          GL_FALSE, // normalized
                          0, // stride
                          (void*) (sizeof(GLfloat) * 8)); // pointer offset

    stbi_set_flip_vertically_on_load(true);
    u8* texture2d_data = NULL;
    GLint width, height, nChannels = 0;
    texture2d_data = stbi_load("./assets/FLAG_B24.BMP", &width, &height, &nChannels, 0);

    // [ cfarvin::DEBUG ]
    printf("[ debug ] STBI Stats:\n\twidth: %d\n\theight: %d\n\tnum channels: %d\n\t",
           width, height, nChannels);

    if(texture2d_data)
    {
        /*
          void glTexImage2D(
          GLenum target,
          GLint level,
          GLint internalFormat,
          GLsizei width,
          GLsizei height,
          GLint border,
          GLenum format,
          GLenum type,
          const GLvoid * data
          );
        */

        glError;
        glBindTexture(GL_TEXTURE_2D, texture_test_renderer->texture_id);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     width,
                     height,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     texture2d_data);

        glGenerateMipmap(GL_TEXTURE_2D);
        glError;
    }
    else
    {
        uError_v("[ ERROR ] Failed to load image texture\n");
        assert(0);
    }

    stbi_image_free(texture2d_data);
    glError;

    // Unbind
    /* glBindTexture(GL_TEXTURE_2D, 0); */
    /* glBindBuffer(GL_ARRAY_BUFFER, 0); */
    /* glBindVertexArray(0); */
    /* glUseProgram(0); */
    glError;
}

void
render_texture_test()
{
    glUseProgram(texture_test_renderer->shader_program);
    glBindVertexArray(texture_test_renderer->vertex_array_object);
    glEnableVertexAttribArray(texture_test_renderer->shdr_position_location);
    glError;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_test_renderer->texture_id);
    glError;

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glError;

    glBindVertexArray(0);
    glEnableVertexAttribArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glError;
}
