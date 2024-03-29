// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

// Texture sampler
uniform sampler2D onetex;
uniform int num_tiles;

void main()
{   
    // Sample texture
    vec4 color = texture2D(onetex, vec2(uv_interp.x*num_tiles, uv_interp.y*num_tiles));

    // Assign color to fragment
    gl_FragColor = vec4(color.r, color.g, color.b, color.a);

    // Check for transparency
    if(color.a < 1.0)
    {
         discard;
    }
}
