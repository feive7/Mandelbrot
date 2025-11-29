#version 330
#define iterations 1000.0
#define PI 3.141592654
// Input texture coords
in vec2 fragTexCoord;

// Uniforms
uniform vec2 point;
uniform int mouse_buttons;
uniform int julia_toggle;

// Output color
out vec4 finalColor;

// Complex functions
#define cx_mul(a, b) vec2(a.x*b.x-a.y*b.y, a.x*b.y+a.y*b.x)
#define cx_div(a, b) vec2(((a.x*b.x+a.y*b.y)/(b.x*b.x+b.y*b.y)),((a.y*b.x-a.x*b.y)/(b.x*b.x+b.y*b.y)))
#define cx_modulus(a) length(a)
#define cx_conj(a) vec2(a.x, -a.y)
#define cx_arg(a) atan(a.y, a.x)
#define cx_sin(a) vec2(sin(a.x) * cosh(a.y), cos(a.x) * sinh(a.y))
#define cx_cos(a) vec2(cos(a.x) * cosh(a.y), -sin(a.x) * sinh(a.y))
#define cx_abs(a) vec2(abs(a.x),abs(a.y))

vec2 cx_sqrt(vec2 a) {
    float r = length(a);
    float rpart = sqrt(0.5*(r+a.x));
    float ipart = sqrt(0.5*(r-a.x));
    if (a.y < 0.0) ipart = -ipart;
    return vec2(rpart,ipart);
}

vec2 cx_tan(vec2 a) {return cx_div(cx_sin(a), cx_cos(a)); }

vec2 cx_log(vec2 a) {
    float rpart = sqrt((a.x*a.x)+(a.y*a.y));
    float ipart = atan(a.y,a.x);
    if (ipart > PI) ipart=ipart-(2.0*PI);
    return vec2(log(rpart),ipart);
}

vec2 cx_mobius(vec2 a) {
    vec2 c1 = a - vec2(1.0,0.0);
    vec2 c2 = a + vec2(1.0,0.0);
    return cx_div(c1, c2);
}

vec2 cx_z_plus_one_over_z(vec2 a) {
    return a + cx_div(vec2(1.0,0.0), a);
}

vec2 cx_z_squared_plus_c(vec2 z, vec2 c) {
    return cx_mul(z, z) + c;
}

vec2 cx_sin_of_one_over_z(vec2 z) {
    return cx_sin(cx_div(vec2(1.0,0.0), z));
}

vec2 cx_square(vec2 z) {
    return vec2(z.x*z.x-z.y*z.y,2*z.x*z.y);
}


// Color conversion
vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

// Domain coloring
vec3 domainColoring (vec2 z) {
  float saturation = 0.9;
  float gridStrength = 0.5;
  float magStrength = 0.7;
  vec2 gridSpacing = vec2(1.0,1.0);
  float linePower = 8.0;

  float carg = atan(z.y, z.x);
  float cmod = length(z);

  float rebrt = (fract(z.x / gridSpacing.x) - 0.5) * 2.0;
  rebrt *= rebrt;

  float imbrt = (fract(z.y / gridSpacing.y) - 0.5) * 2.0;
  imbrt *= imbrt;

  float grid = 1.0 - (1.0 - rebrt) * (1.0 - imbrt);
  grid = pow(abs(grid), linePower);

  float circ = (fract(log2(cmod)) - 0.5) * 2.0;
  circ = pow(abs(circ), linePower);

  circ *= magStrength;

  vec3 rgb = hsv2rgb(vec3(carg * 0.5 / PI, saturation, 0.5 + 0.5 * saturation - gridStrength * grid));
  rgb *= (1.0 - circ);
  rgb += circ * vec3(1.0);
  return rgb;
}

vec4 mandelbrot(vec2 z, vec2 c) {
    int i;
    for(i = 0; i < iterations; i++) {
        if(length(z) > 4) break;
        z = cx_square(cx_abs(z)) + c;
    }
    return vec4(z,float(i),(i==iterations));
}

float magic_formula(vec3 m) {
    return m.z + 1.0 - log(log(m.x * m.x + m.y * m.y) / 2.0 / log(2.0)) / log(2.0);
}

vec3 palette(vec4 result) {
    if (result.w == 1.0) return vec3(0.0);

    float t = magic_formula(result.xyz);

    float hue = fract(t * 0.03);
    float sat = 0.85;
    float val = 1.0;

    return hsv2rgb(vec3(hue, sat, val));
}


void main() {
    vec4 m_result = mandelbrot(vec2(0.0), fragTexCoord);
    vec3 m_color = palette(m_result);
    vec4 j_result = mandelbrot(fragTexCoord,point);
    vec3 j_color = palette(j_result);
    if(julia_toggle == 1) {
        finalColor = vec4(j_color,1.0);
    }
    else if (mouse_buttons == 2) {
        finalColor = vec4(mix(j_color,m_color,0.5),1.0);
    }
    else {
        finalColor = vec4(m_color,1.0);
    }
}
                     