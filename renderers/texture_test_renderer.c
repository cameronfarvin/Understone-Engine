#include <renderers/texture_test_renderer.h>

// [ cfarvin::DEBUG ] [ cfarvin::REMOVE ]
#define STB_IMAGE_IMPLEMENTATION
#include <engine_tools/../test_dep/stb_image.h>


void
initRenderer_texture_test(uGLRenderTarget* const texture_test_renderer)
{
    const char* vshdr = GLSL
        (450 core,

         uniform mat4 vshdr_mut_pos;

         in      vec2 vshdr_pos;
         in      vec2 texture_coordinates_in;

         out     vec2 texture_coordinates;

         void main()
         {
             gl_Position = vshdr_mut_pos * vec4(vshdr_pos,
                                                0.0f,
                                                1.0f);
             texture_coordinates = texture_coordinates_in;
         });

    const char* fshdr = GLSL
        (450 core,

         uniform vec3      fshdr_color;
         uniform sampler2d texture2d;

         in      vec2      texture_coordinates;

         out     vec4      fshdr_final;

         void main()
         {
             fshdr_final = texture(texture2d, texture_coordinates) * vec4(fshdr_color, 1.0f);
         });

    texture_test_renderer->shader_program = 0;
    texture_test_renderer->vshdr_position_location = -1;
    texture_test_renderer->modelview_matrix_location = -1;
    texture_test_renderer->fshdr_color_location = -1;
    texture_test_renderer->fshdr_texture_coords_location = -1;

    // [ cfarvin::RETURN ]
    // Thinking about int assignments on mem vs gc for textures, what they mean.

    GLfloat texture_test_vertex_data[] =
        {
            //
            // vertex coords
            //
            // top right [0]
            0.1, 0.1,

            // top left [1]
            -0.1, 0.1,

            // bottom left [2]
            -0.1, -0.1,

            // bottom right [3]
            0.1, -0.1,

            //
            // texture coords
            //
            // top right
            1.0, 1.0,

            // top left
            -1.0, 1.0,

            // bottom left
            -1.0, -1.0,

            // bottom right
            1.0, -1.0
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
    assert(texture_test_renderer->shader_program);
    glUseProgram(texture_test_renderer->shader_program);
    glError;

    //
    // discover attribute location
    //
    texture_test_renderer->vshdr_position_location =
        glGetAttribLocation(texture_test_renderer->shader_program, "vshdr_pos");
    texture_test_renderer->modelview_matrix_location =
        glGetUniformLocation(texture_test_renderer->shader_program, "vshdr_mut_pos");
    texture_test_renderer->fshdr_color_location =
        glGetUniformLocation(texture_test_renderer->shader_program, "fshdr_color");
    texture_test_renderer->fshdr_texture_coords_location =
        glGetUniformLocation(texture_test_renderer->shader_program, "texture_coordinates_in");;
    glError;

    // set texture num
    glUniform1i(glGetUniformLocation(texture_test_renderer->shader_program, "texture2d"), 0);
    glError;

    assert(texture_test_renderer->vshdr_position_location != -1);
    assert(texture_test_renderer->modelview_matrix_location != -1);
    assert(texture_test_renderer->fshdr_color_location != -1);
    assert(texture_test_renderer->fshdr_texture_coords_location != -1);

    //
    // set_default_attribute_values
    //
    glError;
    glUniform3f(texture_test_renderer->fshdr_color_location, 1.0f, 0.0f, 0.0f);

    glError;
    GLfloat tmp_modelview[16] =
        {
            +1.0f, +0.0f, +0.0f, +0.0f,
            +0.0f, +1.0f, +0.0f, +0.0f,
            +0.0f, +0.0f, +1.0f, +0.0f,
            +0.0f, +0.0f, +0.0f, +1.0f,
        };

    for (size_t ii = 0; ii < 16; ii++)
    {
        texture_test_renderer->modelview_matrix[ii] = tmp_modelview[ii];
    }

    glUniformMatrix4fv(texture_test_renderer->modelview_matrix_location,
                       1,
                       GL_TRUE,
                       texture_test_renderer->modelview_matrix);

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

    glGenTextures(1, &texture_test_renderer->fshdr_texture_test_location);
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

    glVertexAttribPointer(texture_test_renderer->vshdr_position_location,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*) 0);

    glVertexAttribPointer(texture_test_renderer->fshdr_texture_2d_location,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*) (sizeof(GLfloat) * 8));

    stbi_set_flip_vertically_on_load(true);

    glBindTexture(GL_TEXTURE_2D, texture2d);
    u8* texture2d_data;

    GLint width, height, nChannels;
    texture2d_data = stbi_load("./assets/FLAG_B24.BMP", &width, &height, &nChannels, 0);

    if(texture2d_data)
    {
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

    stbi_image_free(texCrateData);
    glError;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    glError;
}

void
render_texture_test(uGLRenderTarget* const texture_test_renderer)
{
    // [ cfarvin::DEBUG ] [ cfarvin::REMOVE ] [ cfarvin::EXPERIMENTAL ]
    static r32 piCycle = 0.0f;
    static r32 cycleDelta = 0.025f;

    if (piCycle > uPI)
    {
        piCycle = 0;
    }

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(texture_test_renderer->shader_program);
    glBindVertexArray(texture_test_renderer->vertex_array_buffer_location);
    glEnableVertexAttribArray(texture_test_renderer->vshdr_position_location);

    texture_test_renderer->modelview_matrix[3] =
        -((viewport.width - mouse_pos.x) / (viewport.width / 2.0f) - 1);
    texture_test_renderer->modelview_matrix[7] =
        -((viewport.height - mouse_pos.y) / (viewport.height / 2.0f) - 1);

    glUniformMatrix4fv(texture_test_renderer->modelview_matrix_location,
                       1,
                       GL_TRUE,
                       texture_test_renderer->modelview_matrix);

    piCycle += cycleDelta;
    if (uGetInputPressed(uMouse_right))
    {
        // red triangle
        glUniform3f(texture_test_renderer->fshdr_color_location, (GLfloat) sin(piCycle), 0.0, 0.0f);
    }
    else if (uGetInputPressed(uMouse_left))
    {
        // blue triangle
        glUniform3f(texture_test_renderer->fshdr_color_location, 0.0f, 0.0f, (GLfloat) sin(piCycle));
    }
    else
    {
        // green triangle
        glUniform3f(texture_test_renderer->fshdr_color_location, 0.0f, (GLfloat) sin(piCycle), 0.0f);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture2d);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    glBindVertexArray(0);
    glEnableVertexAttribArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
