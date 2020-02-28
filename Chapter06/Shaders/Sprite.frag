// request GLSL 3.3
#version 330

// tex coord input from vertex shader
in vec2 fragTexCoord;

// this corresponds to the output color to the color buffer
out vec4 outColor;

// this is used for the texture sampling
uniform sampler2D uTexture;

void main() {
    // sample color from texture
    outColor = texture(uTexture, fragTexCoord);
}
