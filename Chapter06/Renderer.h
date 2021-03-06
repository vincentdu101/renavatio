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

struct DirectionalLight
{
    // Direction of light
    Vector3 mDirection;
    // Diffuse color
    Vector3 mDiffuseColor;
    // Specular color
    Vector3 mSpecColor;
};

struct PointLight
{
    // direction of light
    Vector3 mDirection;
    
    // diffuse color
    Vector3 mDiffuseColor;
    
    // specular color
    Vector3 mSpecColor;
    
    // position of light
    Vector3 mPosition;
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
    PointLight* GetPointLights() { return mPointLights; }

    float GetScreenWidth() const { return mScreenWidth; }
    float GetScreenHeight() const { return mScreenHeight; }
private:
    bool LoadShaders();
    void CreateSpriteVerts();
    void SetLightUniforms(class Shader* shader);

    // Map of textures loaded
    std::unordered_map<std::string, class Texture*> mTextures;
    // Map of meshes loaded
    std::unordered_map<std::string, class Mesh*> mMeshes;

    // All the sprite components drawn
    std::vector<class SpriteComponent*> mSprites;

    // All mesh components drawn
    std::vector<class MeshComponent*> mMeshComps;

    // Game
    class Game* mGame;

    // Sprite shader
    class Shader* mSpriteShader;
    // Sprite vertex array
    class VertexArray* mSpriteVerts;
    
    std::unordered_map<std::string, class Shader*> mShaderMap;

    // Mesh shader
    class Shader* mMeshShader;

    // View/projection for 3D shaders
    Matrix4 mView;
    Matrix4 mProjection;
    // Width/height of screen
    float mScreenWidth;
    float mScreenHeight;

    // Lighting data
    Vector3 mAmbientLight;
    DirectionalLight mDirLight;
    PointLight mPointLights[4];
    float mPointLightsRadius = 200.0f;
    
    // Window
    SDL_Window* mWindow;
    // OpenGL context
    SDL_GLContext mContext;
};

