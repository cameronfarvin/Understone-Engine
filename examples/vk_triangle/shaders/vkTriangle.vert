//
// @AutoGenFileName: vkTriangleShader.h
// @CommonName:      vkTriangle_vert
// @ShaderType:      VERTEX
//

#version 450

// Specify the index of the framebuffer
layout(location = 0) out vec3 fragColor;


vec3 colors[3] = vec3[]
(
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

vec2 positions[3] = vec2[]
(
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

void main()
{
    // gl_VertexIndex: built-in
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    fragColor = colors[gl_VertexIndex];
}

// #version 450

// layout(location = 0) in vec2 in_position;
// layout(location = 1) in vec3 in_color;

// layout(location = 0) out vec3 frag_color;

// void main()
// {
//     // gl_VertexIndex: built-in
//     gl_Position = vec4(in_position, 0.0, 1.0);
//     frag_color = in_color;
// }
