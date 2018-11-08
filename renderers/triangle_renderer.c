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
    glError;
    triangle_shader_program = createShaderProgram_vf(&vshdr, &fshdr);
    glUseProgram(triangle_shader_program);
    assert(triangle_shader_program);
    glError;

    //
    // discover_attribute_locations
    //
    glError;
    triangle_vshdr_position_location = glGetAttribLocation(triangle_shader_program, "vshdr_pos");
    triangle_fshdr_color_location    = glGetUniformLocation(triangle_shader_program, "fshdr_color");
    glError;

    assert(triangle_vshdr_position_location != -1);
    assert(triangle_fshdr_color_location != -1);

    //
    // set_default_attribute_values
    //
    glError;
    glUniform3f(triangle_fshdr_color_location, 1.0f, 0.0f, 0.0f);
    glError;

    glError;
    glGenVertexArrays(1, &triange_vertex_array_bufer_location);
    glBindVertexArray(triange_vertex_array_bufer_location);
    glError;

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
    glError;
    

    //
    //
    // framebuffer_test
    //
    //

    //
    // color_attachment
    //
    glError;
    GLuint texture_map;
    glGenTextures(1, &texture_map);
    glBindTexture(GL_TEXTURE_2D, texture_map);
    glError;

    glError;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glError;

    GLubyte test_fbo_data[100] = { 255 };


    glTexImage2D(GL_TEXTURE_2D,         // GLenum  target
                 0,                     // GLint   level
                 GL_RGBA,               // GLint   internalFormat
                 10,                    // GLsizei width
                 10,                    // GLsizei height
                 0,                     // GLint   border
                 GL_RGBA,               // GLenum  format
                 GL_UNSIGNED_BYTE,      // GLenum  type
                 test_fbo_data);        // const GLvoid* data
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glError;

    //
    // depth_attachment
    //
    glError;
    GLuint depth_texture;
    glGenTextures(1, &depth_texture);
    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glError;

    glError;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glError;

    glError;
    glTexImage2D(GL_TEXTURE_2D,         // GLenum  target
                 0,                     // GLint   level
                 GL_DEPTH_COMPONENT,    // GLint   internalFormat
                 10,                    // GLsizei width
                 10,                    // GLsizei height
                 0,                     // GLint   border
                 GL_DEPTH_COMPONENT,    // GLenum  format
                 GL_UNSIGNED_BYTE,      // GLenum  type
                 test_fbo_data);        // const GLvoid* data
    glError;
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glError;

    //
    // build_fbo
    //
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glError;

    glFramebufferTexture2D(GL_FRAMEBUFFER,       // GLenum target
                           GL_COLOR_ATTACHMENT0, // GLenum attachment
                           GL_TEXTURE_2D,        // GLenum textureTarget
                           texture_map,           // GLuint texture
                           0);                   // Glint  mipmap level
    glError;

    glFramebufferTexture2D(GL_FRAMEBUFFER,       // GLenum target
                           GL_DEPTH_ATTACHMENT,  // GLenum attachment
                           GL_TEXTURE_2D,        // GLenum textureTarget
                           depth_texture,        // GLuint texture
                           0);                   // Glint  mipmap level
    glError;

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);;
    assert(status == GL_FRAMEBUFFER_COMPLETE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
