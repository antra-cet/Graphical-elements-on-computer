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
uniform vec2 text_coords;

// Output
out vec2 texcoord;
out vec3 frag_color;
out vec3 world_position;
out vec3 world_normal;

void main()
{
    world_position = (Model * vec4(v_position, 1)).xyz;
	world_normal = normalize(mat3(Model) * normalize(v_normal));

    texcoord = v_texture_coord;
    texcoord = vec2(texcoord.x + text_coords.x, texcoord.y + text_coords.y);

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
