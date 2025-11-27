#version 330

// Input texture coords
in vec2 fragTexCoord;

// Output color
out vec4 finalColor;

// Square complex number (x+yi)^2
vec2 complex_square(vec2 z) {
    return vec2(z.x*z.x-z.y*z.y,2*z.x*z.y);
}

int mandelbrot(vec2 c) {
    vec2 z = c;
    int i;
    for(i = 0; i < 40; i++) {
        if(length(z) > 4) break;
        z = complex_square(z) + c;
    }
    if(i<40) return i; // Point is in the mandelbrot set
    return 0; // Point is not in the mandelbrot set
}

void main() {
    float value = mandelbrot(fragTexCoord) / 40.0;
    finalColor = vec4(value,value,value,1.0);
}
