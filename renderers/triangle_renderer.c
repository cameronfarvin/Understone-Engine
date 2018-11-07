#include <engine_tools/ogl_tools.h>

//
// vshdr
//
const char* vshdr = GLSL
    (450 core,

     in vec2 vshdr_pos;

     void main()
     {
         gl_Position = vec4(vshdr_pos, 0.0f, 1.0f);
     });

//
// fshdr
//
const char* fshdr = GLSL
    (450 core,

     uniform vec3 fshdr_color;

     out vec4 fshdr_final;

     void main()
     {
         fshdr_final = vec4(fshdr_color, 1.0f);
     });


GLuint triangle_shader_program = 0;

//
// attribute_locations
//
GLint triangle_vshdr_position_location = -1;
GLint triangle_fshdr_color_location = -1;

//
// vao_vbo
//
GLuint triange_vertex_array_bufer_location;
GLuint triangle_vertex_buffer_location;

//
// triangle_vertex_data
//
GLfloat triangle_vertex_data[] =
    {
        // top
        0.0f, 0.5f,

        // bottom left
        -0.5f, -0.5f,

        // bottom right
        0.5f, -0.5f
    };

void
initRenderer_triangle()
{
    //
    // create_program
    //
    glError();
    triangle_shader_program = createShaderProgram_vf(&vshdr, &fshdr);
    glUseProgram(triangle_shader_program);
    assert(triangle_shader_program);
    glError();

    //
    // discover_attribute_locations
    //
    glError();
    triangle_vshdr_position_location = glGetAttribLocation(triangle_shader_program, "vshdr_pos");
    triangle_fshdr_color_location    = glGetUniformLocation(triangle_shader_program, "fshdr_color");
    glError();

    assert(triangle_vshdr_position_location != -1);
    assert(triangle_fshdr_color_location != -1);

    //
    // set_default_attribute_values
    //
    glError();
    glUniform3f(triangle_fshdr_color_location, 1.0f, 0.0f, 0.0f);
    glError();

    glError();
    glGenVertexArrays(1, &triange_vertex_array_bufer_location);
    glBindVertexArray(triange_vertex_array_bufer_location);
    glError();

    glGenBuffers(1, &triangle_vertex_buffer_location);
    glBindBuffer(GL_ARRAY_BUFFER, triangle_vertex_buffer_location);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(triangle_vertex_data),
                 triangle_vertex_data,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(triangle_vshdr_position_location,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*) 0);
    glError();
}

void
render_triangle()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(triangle_shader_program);
    glBindVertexArray(triange_vertex_array_bufer_location);
    glEnableVertexAttribArray(triangle_vshdr_position_location);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
