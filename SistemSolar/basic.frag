 #version 330 core
out vec4 FragColor;
in vec2 bTex;

uniform sampler2D textura_mea;
void main()
{
    FragColor = texture(textura_mea, bTex);//vec4(0.5f, 0.0f, 1.0f, 1.0f);
}


/*
in vec2 bTex;
out vec4 FragColor;
uniform sampler2D testura_mea;

void main()
{
gl_Position
}

*/
