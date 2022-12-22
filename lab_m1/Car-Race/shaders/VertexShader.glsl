#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 car_translate;

// Output
out vec3 frag_normal;
out vec3 frag_position;
out vec3 frag_color;
out vec2 tex_coord;


void main()
{
    frag_normal = v_normal;
    frag_position = v_position;
    frag_color = v_color;
    tex_coord = v_texture_coord;

    vec4 pos = Model * vec4(v_position, 1.0);
    vec3 Posv = vec3(pos.x, pos.y, pos.z);
    float Posvy = Posv.y - length(car_translate - Posv) * length(car_translate - Posv) * 0.00015f;
 
    gl_Position = Projection * View * vec4(Posv.x, Posvy, Posv.z, 1.0);
}
