#include <renderers/texture_test_renderer.h>

// [ cfarvin::DEBUG ] [ cfarvin::REMOVE ]
#define STB_IMAGE_IMPLEMENTATION
#include <engine_tools/../test_dep/stb_image.h>


void
initRenderer_texture_test(uGLRenderTarget* const texture_test_renderer)
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
         fshdr_final = texture(texture2d, texture_coordinates) * vec4(0.0f, 0.0f, 1.0f, 1.0f);
         });

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
        0.25, 0.25,

        // top left [1]
        -0.25, 0.25,

        // bottom left [2]
        -0.25, -0.25,

        // bottom right [3]
        0.25, -0.25,
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
                                                                      &fshdr,
                                                                      __FILE__);
    glError;
    assert(texture_test_renderer->shader_program);
    glUseProgram(texture_test_renderer->shader_program);
    glError;

    //
    // discover attribute location
    //
    texture_test_renderer->shdr_position_location =
        glGetAttribLocation(texture_test_renderer->shader_program, "vshdr_pos");
    texture_test_renderer->shdr_texture_coords_location =
        glGetAttribLocation(texture_test_renderer->shader_program, "texture_coordinates_in");;
    glError;

    // set texture num
    glUniform1i(glGetUniformLocation(texture_test_renderer->shader_program, "texture2d"), 0);
    glError;

    assert(texture_test_renderer->shdr_position_location != -1);
    assert(texture_test_renderer->shdr_texture_coords_location != -1);

    glError;
    glGenVertexArrays(1, &texture_test_renderer->vertex_array_buffer_location);
    glBindVertexArray(texture_test_renderer->vertex_array_buffer_location);

    glError;
    glGenBuffers(1, &texture_test_renderer->element_buffer_location);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texture_test_renderer->element_buffer_location);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(ebo_indices),
                 ebo_indices,
                 GL_STATIC_DRAW);

    glError;
    glGenBuffers(1, &texture_test_renderer->vertex_buffer_location);
    glBindBuffer(GL_ARRAY_BUFFER, texture_test_renderer->vertex_buffer_location);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(texture_test_vertex_data),
                 texture_test_vertex_data,
                 GL_STATIC_DRAW);

    glError;
    glGenTextures(1, &texture_test_renderer->texture_name);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_test_renderer->texture_name);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /*

    // function spec
    void glVertexAttribPointer( GLuint        index,
    GLint         size,
    GLenum        type,
    GLboolean     normalized,
    GLsizei       stride,
    const GLvoid* pointer);

     */

    glError;
    glVertexAttribPointer(texture_test_renderer->shdr_position_location,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*) 0);

    glVertexAttribPointer(texture_test_renderer->shdr_texture_2d_location,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*) (sizeof(GLfloat) * 8));

    stbi_set_flip_vertically_on_load(true);
    u8* texture2d_data = NULL;

    GLint width, height, nChannels;
    texture2d_data = stbi_load("./assets/sails.bmp", &width, &height, &nChannels, 0);
    // texture2d_data = stbi_load("./assets/FLAG.BMP", &width, &height, &nChannels, 0);

    // [ cfarvin::DEBUG ]
    printf("[ debug ][ debug ]\n\twidth: %d\n\theight: %d\n\tnum channels: %d\n\t",
           width, height, nChannels);

    if(texture2d_data)
    {
        /*
           void glTexImage2D(   GLenum target,
           GLint level,
           GLint internalFormat,
           GLsizei width,
           GLsizei height,
           GLint border,
           GLenum format,
           GLenum type,
           const GLvoid * data);
         */

        glError;
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
    }
    else
    {
        printf("[ ERROR ] Failed to load image texture (texCrateData)\n");
    }

    stbi_image_free(texture2d_data);
    glError;

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    glError;
}

void
render_texture_test(uGLRenderTarget* const texture_test_renderer)
{
    glUseProgram(texture_test_renderer->shader_program);
    glBindVertexArray(texture_test_renderer->vertex_array_buffer_location);
    glEnableVertexAttribArray(texture_test_renderer->shdr_position_location);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_test_renderer->texture_name);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glEnableVertexAttribArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
