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
    float gravity = 2.8;

    // Add phase to the time and cycle it
    float cycle = 2.0;
    acttime = mod(time + t*cycle, cycle);

    // Move particle along given direction
    pos = vec4(vertex.x + acttime*speed*dir.x , vertex.y + acttime*speed*dir.y , 0.0, 1.0);

    // Add wobble effect
    //pos = vec4(vertex.x + cos(acttime)*(speed * 1.5)*dir.x , vertex.y + sin(acttime)*(speed*1.5)*dir.y , 0.0, 1.0);

    // Add gravity
    //pos = vec4(vertex.x+dir.x*acttime*speed , vertex.y+dir.y*acttime*speed + 0.5*gravity*acttime*acttime, 0.0, 1.0);

    // No motion, for debug
    //pos = vec4(vertex.x, vertex.y, 0.0, 1.0);

    // Transform vertex position
    gl_Position = view_matrix*transformation_matrix*pos;
    
    // Set color
    //color_interp = vec4(0.5+0.5*cos(4*acttime),0.5*sin(4*acttime)+0.5,0.5, 1.0 / speed);
    color_interp = vec4(t, 0.0, 0.0, 1.0);

    // Transfer texture coordinates
    uv_interp = uv;
}
