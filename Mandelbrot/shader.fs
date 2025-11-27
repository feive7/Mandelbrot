#version 330

// Input texture coords
in vec2 fragTexCoord;

// Output color
out vec4 finalColor;

void main() {
    finalColor = vec4(fragTexCoord,1.0,1.0);
}
