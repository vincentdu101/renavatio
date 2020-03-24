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
    // set the clear color to light grey
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // draw mesh components
    // enable depth buffering/disable alpha blend
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    
    Shader* mPhongShader = mShaderMap.at("Phong");
    
    // set the mesh shader active
    mPhongShader -> SetActive();
    
    // update view-projection matrix
    mPhongShader -> SetMatrixUniform("uViewProj", mView * mProjection);
    
    // update lighting uniforms
    SetLightUniforms(mPhongShader);
    for (auto mc : mMeshComps) {
        mc -> Draw(mPhongShader, "Phong");
    }
    
    // draw basicmesh components
    Shader* basicMeshShader = mShaderMap.at("BasicMesh");
    basicMeshShader -> SetActive();
    basicMeshShader -> SetMatrixUniform("uViewProj", mView * mProjection);
    
    // update lighting uniforms
    SetLightUniforms(basicMeshShader);
    for (auto mc : mMeshComps) {
        mc -> Draw(basicMeshShader, "BasicMesh");
    }
    
    // draw all sprite components
    // disable depth buffering
    glDisable(GL_DEPTH_TEST);
    
    // enable alpha blending on the color buffer
    glEnable(GL_BLEND);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    
    // set shader/vao as active
    Shader* mSprite = mShaderMap.at("Sprite");
    mSprite -> SetActive();
    mSpriteVerts -> SetActive();
    for (auto sprite : mSprites) {
        sprite -> Draw(mSprite);
    }
    
    // swap the buffers
    SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite) {
    // find the insertion point in the sorted vector
    // the first element with a higher draw order than me
    int myDrawOrder = sprite -> GetDrawOrder();
    auto iter = mSprites.begin();
    for (;
         iter != mSprites.end();
         ++iter) {
        if (myDrawOrder < (*iter) -> GetDrawOrder()) {
            break;
        }
    }
    
    // inserts element before position of iterator
    mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite) {
    auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
    mSprites.erase(iter);
}

void Renderer::AddMeshComp(MeshComponent* mesh) {
    mMeshComps.emplace_back(mesh);
}

void Renderer::RemoveMeshComp(MeshComponent* mesh) {
    auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
    mMeshComps.erase(iter);
}

Texture* Renderer::GetTexture(const std::string& fileName) {
    Texture* tex = nullptr;
    auto iter = mTextures.find(fileName);
    if (iter != mTextures.end()) {
        tex = iter -> second;
    } else {
        tex = new Texture();
        if (tex -> Load(fileName)) {
            mTextures.emplace(fileName, tex);
        } else {
            delete tex;
            tex = nullptr;
        }
    }
    return tex;
}

Mesh* Renderer::GetMesh(const std::string & fileName) {
    Mesh* m = nullptr;
    auto iter = mMeshes.find(fileName);
    if (iter != mMeshes.end()) {
        m = iter -> second;
    } else {
        m = new Mesh();
        if (m -> Load(fileName, this)) {
            mMeshes.emplace(fileName, m);
        } else {
            delete m;
            m = nullptr;
        }
    }
    
    return m;
}

bool Renderer::LoadShaders() {
    // create sprite shader
    mSpriteShader = new Shader();
    
    if (!mSpriteShader -> Load("Shaders/Sprite.vert", "Shaders/Sprite.frag")) {
        return false;
    }
    
    mSpriteShader -> SetActive();
    
    // set the view-projection matrix
    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
    mSpriteShader -> SetMatrixUniform("uViewProj", viewProj);
    mShaderMap.emplace("Sprite", mSpriteShader);
    
    // set the view-projection matrix
    mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
    mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
                                                mScreenWidth,
                                                mScreenHeight,
                                                25.0f,
                                                10000.0f);
    
    // create basic mesh shader
    Shader* meshShader = new Shader();
    if (!meshShader -> Load("Shaders/BasicMesh.vert", "Shaders/BasicMesh.frag")) {
        return false;
    }
    meshShader -> SetActive();
    meshShader -> SetMatrixUniform("uViewProj", mView * mProjection);
    mShaderMap.emplace("BasicMesh", meshShader);
    
    // create phong shader
    mMeshShader = new Shader();
    if (!mMeshShader -> Load("Shaders/Phong.vert", "Shaders/Phong.frag")) {
        return false;
    }
    mMeshShader -> SetActive();
    mMeshShader -> SetMatrixUniform("uViewProj", mView * mProjection);
    mShaderMap.emplace("Phong", mMeshShader);
    return true;
}

void Renderer::CreateSpriteVerts() {
    
    float vertices[] = {
        -0.5f, 0.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, // top left
        0.5f, 0.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, // top right
        0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, // top left
        -0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, // top left
    };
    
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
    
}

void Renderer::SetLightUniforms(Shader* shader) {
    // camera position is from inverted view
    Matrix4 invView = mView;
    invView.Invert();
    shader -> SetVectorUniform("uCameraPos", invView.GetTranslation());
    // ambient light
    shader -> SetVectorUniform("uAmbientLight", mAmbientLight);
    // directional light
    shader -> SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection);
    shader -> SetVectorUniform("uDirLight.mDiffuseColor", mDirLight.mDiffuseColor);
    shader -> SetVectorUniform("uDirLight.mSpecColor", mDirLight.mSpecColor);
}


