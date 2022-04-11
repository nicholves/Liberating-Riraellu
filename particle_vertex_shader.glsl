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

// Attributes forwarded to the fragment shader
out vec4 color_interp;
out vec2 uv_interp;

void main()
{
    vec4 pos;
    float acttime;
    float speed = 4.0;

    // Add phase to the time and cycle it
    float cycle = 2.0;
    acttime = mod(time + t*cycle, cycle);   

    //wobbly effect
    pos = vec4(vertex.x + cos(acttime * 0.01)*speed*dir.x , vertex.y + sin(acttime)*speed*dir.y , 0.0, 1.0);
   

    // Transform vertex position
    gl_Position = view_matrix*transformation_matrix*pos;
    
    // Set color
    color_interp = vec4(t, 0.0, 0.0, 1.0);

    // Transfer texture coordinates
    uv_interp = uv;
}
