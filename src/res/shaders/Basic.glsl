// Basic.shader
#shader vertex
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;

out vec2 v_TextCoord;

void main() {
    gl_Position = position; // position ya es vec4, no necesitas "vec4(position, 0.0, 1.0)"
    v_TextCoord = texCoord; // <--- ¡Asigna el valor de texCoord a v_TextCoord!
}

#shader fragment
#version 330 core
layout (location = 0) out vec4 color;

in vec2 v_TextCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main(){
    vec4 texColor = texture(u_Texture, v_TextCoord); // <--- Función "texture()" y añade ";"
    color = texColor * u_Color; // <--- Multiplica por u_Color para ver el efecto del uniforme
}