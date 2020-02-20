//
//  Renderer.cpp
//  Game-mac
//
//  Created by Vincent Du on 2/19/20.
//  Copyright © 2020 Sanjay Madhav. All rights reserved.
//

#include "Renderer.h"
#include "Texture.h"
#include "Mesh.h"
#include <algorithm>
#include "Shader.h"
#include "VertexArray.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include <GL/glew.h>

Renderer::Renderer(Game* game)
    :mGame(game)
    ,mSpriteShader(nullptr)
    ,mMeshShader(nullptr)
{
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize(float screenWidth, float screenHeight)
{
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;
    
    // set opengl attributes
    // use the core opengl profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    // specify version 3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    // request a color buffer with 8 bits per RGBA channel
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    // enable double buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    // force opengl to use hardware acceleration
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    
    mWindow = SDL_CreateWindow("Renavatio", 100, 100,
                               static_cast<int>(mScreenWidth),
                               static_cast<int>(mScreenHeight),
                               SDL_WINDOW_OPENGL);
    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
    
    // create an opengl context
    mContext = SDL_GL_CreateContext(mWindow);
    
    // initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        SDL_Log("Failed to initialize GLEW.");
        return false;
    }
    
    // on some platforms, GLEW will emit a benign error code, so
    // clear it
    glGetError();
    
    // make sure we can create/compile shaders
    if (!LoadShaders()) {
        SDL_Log("failed to load shaders");
        return false;
    }
    
    // create quad for drawing sprites
    CreateSpriteVerts();
    
    return true;
}

void Renderer::Shutdown()
{
    delete mSpriteVerts;
    mSpriteShader -> Unload();
    delete mSpriteShader;
    mMeshShader -> Unload();
    delete mMeshShader;
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);
}

void Renderer::UnloadData()
{
    // destroy textures
    for (auto i : mTextures) {
        i.second -> Unload();
        delete i.second;
    }
    mTextures.clear();
    
    // Destroy meshes
    for (auto i : mMeshes)
    {
        i.second->Unload();
        delete i.second;
    }
    mMeshes.clear();
}

void Renderer::Draw() {
    
}
