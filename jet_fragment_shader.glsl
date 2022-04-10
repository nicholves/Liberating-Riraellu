// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;
in float speed_colour;

// Texture sampler
uniform sampler2D onetex;

void main()
{
    // Sample texture
    vec4 color = texture2D(onetex, uv_interp);
    if(speed_colour > 2.5){
        color.rgb = vec3(0.02, 0.02, 0.75) * color_interp.r;
    }
    else if(speed_colour > 1.0){
        color.rgb = vec3(0.01, 0.01, 0.5) * color_interp.r;
    }
    else{
        color.rgb = vec3(0.001, 0.001, 0.01) * color_interp.r;
    }

    // Assign color to fragment
    gl_FragColor = vec4(color.r, color.g, color.b, color.a);

    // Check for transparency
    //if(color.a < 1.0)
    //{
    //     discard;
    //}
}
