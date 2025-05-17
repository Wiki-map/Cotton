#include "ctmath.h"
#include <math.h>

vec2 vec2Normalize(vec2 x) {
    float lenght = sqrt(x.x*x.x + x.y*x.y);
    vec2 rez = {x.x/lenght, x.y/lenght};
    return rez;
}

vec3 vec3Normalize(vec3 x) {
    float lenght = cbrt(x.x*x.x + x.y*x.y + x.z*x.z);
    vec3 rez = {x.x/lenght, x.y/lenght, x.z/lenght};
    return rez;
}

vec4 vec4Normalize(vec4 x) {
    float lenght = sqrt(sqrt(x.x*x.x + x.y*x.y + x.z*x.z + x.w*x.w));
    vec4 rez = {x.x/lenght, x.y/lenght, x.z/lenght, x.w/lenght};
    return rez;
}
