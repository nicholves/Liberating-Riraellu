// Source code of vertex shader for particle system
#version 130

// Vertex buffer
in vec2 vertex;
in vec2 dir;
in float t; //phase
in vec2 uv;

// Uniform (global) buffer
uniform mat4 transformation_matrix;
uniform mat4 view_matrix;
uniform float time;
uniform float speed;

// Attributes forwarded to the fragment shader
out vec4 color_interp;
out vec2 uv_interp;
out float speed_colour;

void main()
{
    vec4 pos;
    float acttime;
    speed_colour = speed;    

    // Add phase to the time and cycle it
    float cycle = 2.0;
    acttime = mod(time + t*cycle, cycle);

    // Move particle along given direction
    pos = vec4(vertex.x + acttime*speed*dir.x , vertex.y + acttime*speed*dir.y , 0.0, 1.0);    

    // Transform vertex position
    gl_Position = view_matrix*transformation_matrix*pos;
    
    // Set color    
    color_interp = vec4(t, 0.0, 0.0, 1.0);

    // Transfer texture coordinates
    uv_interp = uv;
}
