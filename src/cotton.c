#include "cotton.h"
#include "ctgl.h"
#include "stdio.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include <math.h>
#include <stdlib.h>

typedef struct State {
    GLFWwindow* window;
    struct{
        bool currentState[512];
        bool lastState[512];
    }keybord;
    clRenderer* mainRenderer;
}clState;

static clState State;

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) State.keybord.currentState[key] = 1;
    if (action == GLFW_RELEASE) State.keybord.currentState[key] = 0;
}

static void SizeCallback(GLFWwindow* window, int width,int height) {
    glViewport(0,0,width,height);
}

void GLAPIENTRY MessageCallback(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userParam ){
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),type, severity, message );
}

void IntiWindow(int ScreenHeight,int ScreenWidth,const char* title) {
    if (!glfwInit()) {
        printf("[ERROR]: failed to inti glfw\n");
    }

    glfwWindowHint(GLFW_RESIZABLE,true);

    State.window = glfwCreateWindow(ScreenHeight,ScreenWidth,title,NULL,NULL);
    if (!State.window) {
        printf("[ERROR]: failed to create window\n");
    }

    glfwSetKeyCallback(State.window, KeyCallback);
    glfwSetFramebufferSizeCallback(State.window,SizeCallback);

    glfwMakeContextCurrent(State.window);
    int gladversion = gladLoadGL(glfwGetProcAddress);
    if (!gladversion) {
        printf("[ERROR]: failed to load opengl\n");
    }

    printf("Loaded opengl, version: %d.%d \n", GLAD_VERSION_MAJOR(gladversion),GLAD_VERSION_MINOR(gladversion));

    State.mainRenderer = malloc(sizeof(clRenderer));
    clRendererInit(State.mainRenderer);
}

static void SwapKeybordStates() {
    for (int i=0; i<512; i++) {
        State.keybord.lastState[i] = State.keybord.currentState[i];
    }
}

bool WindowShouldClose() {
    SwapKeybordStates();
    glfwSwapBuffers(State.window);
    glfwPollEvents();
    return glfwWindowShouldClose(State.window);
}

GLFWwindow* GetWindow() {
    return State.window;
}

bool IsKeyDown(int Key) {
    return State.keybord.currentState[Key];
}

bool IsKeyPresed(int Key) {
    return State.keybord.currentState[Key] == 1 && State.keybord.lastState[Key] == 0;
}

void CleanWindow() {
    free(State.mainRenderer);
}

static vec2 NormalizeCords(vec2 p,int height,int width) {
    return (vec2){p.x/width,p.y/height};
}

static vec2 GetOpenGLCords(vec2 p) {
    int height,width;
    glfwGetWindowSize(State.window,&width, &height);
    float wW = (float)width / 2,wH = (float)height/2;
    float distX = p.x - wW;
    float distY = wH - p.y;
    return (vec2){distX / wW,distY / wH};
}

static vec2 normalizeUV(vec2 p,int width,int height) {
    return (vec2) {0 - p.x,height - p.y};
}

void DrawTriangle(vec2 p1,vec2 p2,vec2 p3,Color color) {
    p1 = GetOpenGLCords(p1);
    p2 = GetOpenGLCords(p2);
    p3 = GetOpenGLCords(p3);

    vec3 _p1 = {p1.x,p1.y,0};
    vec3 _p2 = {p2.x,p2.y,0};
    vec3 _p3 = {p3.x,p3.y,0};

    vec4 _color = {(float)color.r / 255,(float)color.g / 255,(float)color.b / 255,(float)color.a / 255};

    clRendererPushTriangle(State.mainRenderer,_p1,_p2,_p3,_color,_color,_color);
}

void DrawRenctangle(int x,int y,int h,int w,Color color) {
    vec2 p1 = {x,y};
    vec2 p2 = {x,y+h};
    vec2 p3 = {x+w,y};
    vec2 p4 = {x+w,y+h};

    p1 = GetOpenGLCords(p1);
    p2 = GetOpenGLCords(p2);
    p3 = GetOpenGLCords(p3);
    p4 = GetOpenGLCords(p4);

    vec3 _p1 = {p1.x,p1.y,0};
    vec3 _p2 = {p2.x,p2.y,0};
    vec3 _p3 = {p3.x,p3.y,0};
    vec3 _p4 = {p4.x,p4.y,0};

    vec4 _color = {(float)color.r / 255,(float)color.g / 255,(float)color.b / 255,(float)color.a / 255};
 

    clRendererPushTriangle(State.mainRenderer,_p1,_p2,_p3,_color,_color,_color);
    clRendererPushTriangle(State.mainRenderer,_p2,_p3,_p4,_color,_color,_color);
}

void DrawCircle(int x,int y,int r,Color color) {
    float stepSize = 50.0f / r;

    vec4 _color = {(float)color.r / 255,(float)color.g / 255,(float)color.b / 255,(float)color.a / 255};
    vec2 p3 = {x,y};
    p3 = GetOpenGLCords(p3);
    vec3 _p3 = {p3.x,p3.y,0};

    for (float i=stepSize; i<=360; i+=stepSize) {
        vec2 p1 = {
            x + cos(i)*r,
            y + sin(i)*r
        };
        vec2 p2 = {
            x + cos(i-1)*r,
            x + sin(i-1)*r
        };

        p1 = GetOpenGLCords(p1);
        p2 = GetOpenGLCords(p2);

        vec3 _p1 = {p1.x,p1.y,0};
        vec3 _p2 = {p2.x,p2.y,0};

        clRendererPushTriangle(State.mainRenderer,_p1,_p2,_p3,_color,_color,_color);
    }
}

void DrawTexture(Texture2D texture,int x,int y,Color tint) {
    int h = texture.height;
    int w = texture.width;

    vec2 p1 = {x,y};
    vec2 p2 = {x,y+h};
    vec2 p3 = {x+w,y};
    vec2 p4 = {x+w,y+h};

    p1 = GetOpenGLCords(p1);
    p2 = GetOpenGLCords(p2);
    p3 = GetOpenGLCords(p3);
    p4 = GetOpenGLCords(p4);

    vec3 _p1 = {p1.x,p1.y,0};
    vec3 _p2 = {p2.x,p2.y,0};
    vec3 _p3 = {p3.x,p3.y,0};
    vec3 _p4 = {p4.x,p4.y,0};

    vec4 _color = {(float)tint.r / 255,(float)tint.g / 255,(float)tint.b / 255,(float)tint.a / 255};

    vec2 uv1 = {0,1};
    vec2 uv2 = {1,1};
    vec2 uv3 = {0,0};
    vec2 uv4 = {1,0};

    clRendererPushTriangleTexture2D(State.mainRenderer,_p1,_p2,_p3,_color,_color,_color,texture,uv1,uv2,uv3);
    clRendererPushTriangleTexture2D(State.mainRenderer,_p2,_p3,_p4,_color,_color,_color,texture,uv2,uv3,uv4);
}

void DrawTextureRec(Texture2D texture,int x,int y,Rectangle source,Color tint) {
    int h = source.h;
    int w = source.w;

    vec2 p1 = {x,y};
    vec2 p2 = {x,y+h};
    vec2 p3 = {x+w,y};
    vec2 p4 = {x+w,y+h};

    p1 = GetOpenGLCords(p1);
    p2 = GetOpenGLCords(p2);
    p3 = GetOpenGLCords(p3);
    p4 = GetOpenGLCords(p4);

    vec3 _p1 = {p1.x,p1.y,0};
    vec3 _p2 = {p2.x,p2.y,0};
    vec3 _p3 = {p3.x,p3.y,0};
    vec3 _p4 = {p4.x,p4.y,0};

    vec4 _color = {(float)tint.r / 255,(float)tint.g / 255,(float)tint.b / 255,(float)tint.a / 255};

    vec2 uv1 = {source.x,source.y};
    vec2 uv2 = {source.x,source.y+source.h};
    vec2 uv3 = {source.x+source.w,source.y};
    vec2 uv4 = {source.x+source.w,source.y+source.h};
 
    vec2 _uv1 = normalizeUV(uv1,texture.width,texture.height);
    vec2 _uv2 = normalizeUV(uv2,texture.width,texture.height);
    vec2 _uv3 = normalizeUV(uv3,texture.width,texture.height);
    vec2 _uv4 = normalizeUV(uv4,texture.width,texture.height);

    clRendererPushTriangleTexture2D(State.mainRenderer,_p1,_p2,_p3,_color,_color,_color,texture,_uv1,_uv2,_uv3);
    clRendererPushTriangleTexture2D(State.mainRenderer,_p2,_p3,_p4,_color,_color,_color,texture,_uv2,_uv3,_uv4);
}

void BeginDrawing() {
    clRendererBeginDrawing(State.mainRenderer);
}

void EndDrawing() {
    clRendererEndDrawing(State.mainRenderer);
}
void ClearBackground(Color color) {
    vec4 _color = {(float)color.r / 255,(float)color.g / 255,(float)color.b / 255,(float)color.a / 255};
    clClearBackground(_color);
}

Texture2D LoadTexture(const char* path) {return clLoadTexture(path);}

vec2 GetWindowSize() {
    int width,height;
    glfwGetWindowSize(State.window,&width,&height);
    return (vec2){width,height};
}
