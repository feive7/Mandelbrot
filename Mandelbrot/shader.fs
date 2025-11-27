#version 330
#define iterations 1000.0
// Input texture coords
in vec2 fragTexCoord;

// Uniforms
uniform vec2 point;

// Output color
out vec4 finalColor;

// Square complex number (x+yi)^2
vec2 complex_square(vec2 z) {
    return vec2(z.x*z.x-z.y*z.y,2*z.x*z.y);
}

float mandelbrot(vec2 c) {
    vec2 z = point;
    int i;
    for(i = 0; i < iterations; i++) {
        if(length(z) > 4) break;
        z = complex_square(z) + c;
    }
    if(i==iterations) return 0.0;
    return i / iterations;
}

void main() {
    float value = sqrt(mandelbrot(fragTexCoord));
    finalColor = vec4(value,value,value,1.0);
}
