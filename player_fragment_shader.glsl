// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;
in float cloakedFrag;
// Texture sampler
uniform sampler2D onetex;


void main()
{   
    // Sample texture
    vec4 color = texture2D(onetex, vec2(uv_interp.x, uv_interp.y));

    // Assign color to fragment
    if (cloakedFrag == 0.5){
        gl_FragColor = vec4(0, color.g, color.b, color.a);
    }
    else {
        gl_FragColor = vec4(color.r, color.g, color.b, color.a);
    }
    

    // Check for transparency
    if(color.a < 1.0)
    {
         discard;
    }
}
