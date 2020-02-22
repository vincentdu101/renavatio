//
//  Renderer.h
//  Game-mac
//
//  Created by Vincent Du on 2/19/20.
//  Copyright © 2020 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL/SDL.h>
#include "Math.h"

struct DirectionalLight {
    // direction of light
    Vector3 mDirection;
    // diffuse color
    Vector3 mDiffuseColor;
    // specular color
    Vector3 mSpecColor;
};

class Renderer
{
public:
    Renderer(class Game* game);
    ~Renderer();
    
    bool Initialize(float screenWidth, float screenHeight);
    void Shutdown();
    void UnloadData();
    
    void Draw();
    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);
    
    void AddMeshComp(class MeshComponent* mesh);
    void RemoveMeshComp(class MeshComponent* mesh);
    
    class Texture* GetTexture(const std::string& fileName);
    class Mesh* GetMesh(const std::string& fileName);
    
    void SetViewMatrix(const Matrix4& view) { mView = view; }
    void SetAmbientLight(const Vector3& ambient) { mAmbientLight = ambient; }
    DirectionalLight& GetDirectionalLight() { return mDirLight; }
    
    float GetScreenWidth() const { return mScreenWidth; }
    float GetScreenHeight() const { return mScreenHeight; }
private:
    bool LoadShaders();
    void CreateSpriteVerts();
    void SetLightUniforms(class Shader* shader);
    
    // map of textures loaded
    std::unordered_map<std::string, class Texture*> mTextures;
    
    // map of meshes loaded
    std::unordered_map<std::string, class Mesh*> mMeshes;
    
    // all the sprite components drawn
    std::vector<class SpriteComponent*> mSprites;
    
    // all mesh components drawn
    std::vector<class MeshComponent*> mMeshComps;
    
    // Game
    class Game* mGame;
    
    // sprite shader
    class Shader* mSpriteShader;
    
    // sprite vertex array
    class VertexArray* mSpriteVerts;
    
    // mesh shader
    class Shader* mMeshShader;
    
    // view/projection for 3d shaders
    Matrix4 mView;
    Matrix4 mProjection;
    
    // width/height of screen
    float mScreenWidth;
    float mScreenHeight;
    
    // lighting data
    Vector3 mAmbientLight;
    DirectionalLight mDirLight;
    
    // window
    SDL_Window* mWindow;
    
    // opengl context
    SDL_GLContext mContext;
};
