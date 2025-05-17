#include "ctgl.h"
#include <stddef.h>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static bool CTGL_WARNINGS = false;

void clEnableWarnigns(bool val) {
    CTGL_WARNINGS = true;
}

Shader clCompileShader(const char* VertexShaderSource, const char* FragmentShaderSource) {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&VertexShaderSource,NULL);
    glCompileShader(vertexShader);

    int  successVertex;
    char infoLogVertex[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVertex);

    if(!successVertex) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLogVertex);
        printf("[ERROR]: vertex shader error: ");
        printf("%s\n", infoLogVertex);
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&FragmentShaderSource,NULL);
    glCompileShader(fragmentShader);

    int  successFragment;
    char infoLogFragment[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFragment);

    if(!successFragment) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLogFragment);
        printf("[ERROR]: fragment shader error: ");
        printf("%s\n", infoLogFragment);
    }

    int program = glCreateProgram();
    glAttachShader(program,vertexShader);
    glAttachShader(program,fragmentShader);
    glLinkProgram(program);

    int error;
    glGetProgramiv(program,GL_LINK_STATUS,&error);
    if (error == GL_FALSE) {
        printf("[ERROR]: program linking failed\n");
        char buffer[512];
        glGetProgramInfoLog(program,512,NULL,buffer);
        printf("%s\n",buffer);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

Texture2D clLoadTexture(const char* path) {
    Texture rez;
    glGenTextures(1,&rez);
    glBindTexture(GL_TEXTURE_2D, rez);

    int width,height,nrChannels;
    unsigned char* data = stbi_load(path,&width,&height,&nrChannels,0);

    if (!data) {
        printf("[ERROR]: Failed to load texture with id:%d", rez);
        return (Texture2D){0,0,0};
    }

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return (Texture2D){rez,width,height};
}

static char* clGetSourceCode(const char* path) {
    FILE* file = fopen(path,"rb");
    fseek(file, 0, SEEK_END);
    int fileSize = ftell(file);
    rewind(file);
    char* buffer = malloc(sizeof(char) * (fileSize+1));
    fread(buffer,1,fileSize,file);
    buffer[fileSize] = '\0';

    fclose(file);

    return buffer;
}

void clRendererInit(clRenderer* r) {
    glGenBuffers(1,&r->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);

    glGenVertexArrays(1,&r->vao);
    glBindVertexArray(r->vao);

    glBufferData(GL_ARRAY_BUFFER,MAX_VERTICES*sizeof(clVertex),NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(clVertex),(void*) offsetof(clVertex,position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE, sizeof(clVertex), (void*)offsetof(clVertex, color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(clVertex),(void*)offsetof(clVertex, UV));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3,1,GL_FLOAT,GL_FALSE,sizeof(clVertex),(void*)offsetof(clVertex, texIndex));
    glEnableVertexAttribArray(3);

    char* vertexSource = clGetSourceCode(RESOURCE_PATH "vertex.glsl");
    char* fragmentSource = clGetSourceCode(RESOURCE_PATH "fragment.glsl");
    r->shader = clCompileShader(vertexSource,fragmentSource);

    free(vertexSource);
    free(fragmentSource);

    glUseProgram(r->shader);
    uint32_t u_texLocation = glGetUniformLocation(r->shader,"u_tex");
    int32_t textures[8] = {0,1,2,3,4,5,6,7};
    glUniform1iv(u_texLocation,8,textures);
}

void clRendererBeginDrawing(clRenderer* r) {
    r->triangleCount = 0;
    r->textureCount = 0;
}

void clRendererPushTriangleTexture2D(clRenderer* r,vec3 p1,vec3 p2,vec3 p3,vec4 c1,vec4 c2,vec4 c3,Texture2D texture,vec2 uv1,vec2 uv2,vec2 uv3) {

    int tex_pos = -1;
    for (int i=0; i<r->textureCount; i++) {
        if (r->textures[i] == texture.ID) {
            tex_pos = i;
            break;
        }
    }

    if (tex_pos == -1 && r->textureCount < 8) {
        r->textures[r->textureCount] = texture.ID;
        tex_pos = r->textureCount;
        r->textureCount++;
    }

    if (r->triangleCount == MAX_TRIANGLES || tex_pos == -1) {
        if (CTGL_WARNINGS) printf("[WARNING]: batch overflow, starting new batch...\n");
        clRendererEndDrawing(r);
        clRendererBeginDrawing(r);
    }

    r->renderData[r->triangleCount*3].position = p1;
    r->renderData[r->triangleCount*3+1].position = p2;
    r->renderData[r->triangleCount*3+2].position = p3;

    r->renderData[r->triangleCount*3].color = c1;
    r->renderData[r->triangleCount*3+1].color = c2;
    r->renderData[r->triangleCount*3+2].color = c3;

    r->renderData[r->triangleCount*3].UV = uv1;
    r->renderData[r->triangleCount*3+1].UV = uv2;
    r->renderData[r->triangleCount*3+2].UV = uv3;

    r->renderData[r->triangleCount*3].texIndex = tex_pos;
    r->renderData[r->triangleCount*3+1].texIndex = tex_pos;
    r->renderData[r->triangleCount*3+2].texIndex = tex_pos;

    r->triangleCount++;
}

void clRendererPushTriangle(clRenderer* r,vec3 p1,vec3 p2,vec3 p3,vec4 c1,vec4 c2,vec4 c3) {
    Texture texture = clGetWhiteTexture();
    int tex_pos = -1;
    for (int i=0; i<r->textureCount; i++) {
        if (r->textures[i] == texture) {
            tex_pos = i;
            break;
        }
    }

    if (tex_pos == -1 && r->textureCount < 8) {
        r->textures[r->textureCount] = texture;
        tex_pos = r->textureCount;
        r->textureCount++;
    }

    if (r->triangleCount == MAX_TRIANGLES || tex_pos == -1) {
        if (CTGL_WARNINGS) printf("[WARNING]: batch overflow, starting new batch...\n");
        clRendererEndDrawing(r);
        clRendererBeginDrawing(r);
    }

    r->renderData[r->triangleCount*3].position = p1;
    r->renderData[r->triangleCount*3+1].position = p2;
    r->renderData[r->triangleCount*3+2].position = p3;

    r->renderData[r->triangleCount*3].color = c1;
    r->renderData[r->triangleCount*3+1].color = c2;
    r->renderData[r->triangleCount*3+2].color = c3;

    r->renderData[r->triangleCount*3].UV = (vec2){1,1};
    r->renderData[r->triangleCount*3+1].UV = (vec2){1,1};
    r->renderData[r->triangleCount*3+2].UV = (vec2){1,1};

    r->renderData[r->triangleCount*3].texIndex = tex_pos;
    r->renderData[r->triangleCount*3+1].texIndex = tex_pos;
    r->renderData[r->triangleCount*3+2].texIndex = tex_pos;

    r->triangleCount++;
}

void clRendererEndDrawing(clRenderer* r) {

    for (int i=0; i<r->textureCount; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D,r->textures[i]);
    }

    glBindBuffer(GL_ARRAY_BUFFER,r->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, r->triangleCount*3*sizeof(clVertex), r->renderData);
    glUseProgram(r->shader);

    glBindVertexArray(r->vao);
    glDrawArrays(GL_TRIANGLES,0,r->triangleCount*3);
}

void clClearBackground(vec4 color) {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(color.x,color.y,color.z,color.w);
}

Texture WHITE_TEXTURE = 1028;

Texture clGetWhiteTexture() {
    if (WHITE_TEXTURE == 1028) {
        Texture tex;
        uint8_t img[4] = {255,255,255,255};
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        WHITE_TEXTURE = tex;
    }
    return WHITE_TEXTURE;
}
