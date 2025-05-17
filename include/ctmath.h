#ifndef CTMATH
#define CTMATH

#include "stdint.h"

typedef struct vec2 {
    float x;
    float y;
}vec2;

typedef struct vec3 {
    float x;
    float y;
    float z;
}vec3;

typedef struct vec4 {
    float x;
    float y;
    float z;
    float w;
}vec4;

typedef struct Rectangle {
    int x,y,h,w;
}Rectangle;

#if defined(__cplusplus)
extern "C" {
#endif

vec2 vec2Normalize(vec2 x);
vec3 vec3Normalize(vec3 x);
vec4 vec4Normalize(vec4 x);

#if defined(__cplusplus)
}
#endif

#endif
