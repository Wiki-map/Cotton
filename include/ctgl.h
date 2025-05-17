/**************************************************************************************
 *
 *  ctgl v1.0 - a simple opengl abstraction layer
 *
 *  How to use:
 *    RendererInti intitializes a renderer and RendererPushTriangle adds a triangle to
 *    the current batch
 *
 *
 *  Notes:
 *    this project was very inspired by the project https://github.com/PixelRifts/easy-renderer/tree/master, and the raylib library
 *
 **************************************************************************************/
#ifndef CTGL
#define CTGL

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "ctmath.h"
#include "stdint.h"
#include "stdbool.h"

#define Shader uint32_t
#define Texture uint32_t

#define MAX_TRIANGLES 2048
#define MAX_VERTICES 3 * MAX_TRIANGLES

typedef struct clVertex {
    vec3 position;
    vec4 color;
    vec2 UV;
    float texIndex;
}clVertex;

typedef struct Texture2D {
    Texture ID;
    int width;
    int height;
}Texture2D;

typedef struct clRenderer {
    uint32_t vao;
    uint32_t vbo;
    Shader shader;

    clVertex renderData[MAX_VERTICES];
    size_t triangleCount;

    Texture textures[8];
    size_t textureCount;
}clRenderer;

#if defined(__cplusplus)
extern "C" {
#endif


void clEnableWarnigns(bool val);

Shader clCompileShader(const char* VertexShaderSource, const char* FragmentShaderSource);
Texture2D clLoadTexture(const char* path);

void clRendererInit(clRenderer* r); // intitializes the renderer
void clRendererBeginDrawing(clRenderer* r); // begins the batch
void clRendererPushTriangleTexture2D(clRenderer* r,vec3 p1,vec3 p2,vec3 p3,vec4 c1,vec4 c2,vec4 c3,Texture2D texture,vec2 uv1,vec2 uv2,vec2 uv3); // pushes a triangle with a texute to the batch
void clRendererPushTriangle(clRenderer* r,vec3 p1,vec3 p2,vec3 p3,vec4 c1,vec4 c2,vec4 c3); //pushes a triangle to the batch
void clRendererEndDrawing(clRenderer* r); // flushes the renderer
void clClearBackground(vec4 color); // clears background

Texture clGetWhiteTexture(); // return a 1x1 white texture

#if defined(__cplusplus)
}
#endif

#endif
