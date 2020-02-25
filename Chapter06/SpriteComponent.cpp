//
//  SpriteComponent.cpp
//  Game-mac
//
//  Created by Vincent Du on 2/25/20.
//  Copyright © 2020 Sanjay Madhav. All rights reserved.
//

#include "SpriteComponent.h"
#include "Texture.h"
#include "Shader.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
    :Component(owner)
    ,mTexture(nullptr)
    ,mDrawOrder(drawOrder)
    ,mTexWidth(0)
    ,mTexHeight(0)
{
    mOwner -> GetGame() -> GetRenderer() -> AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
    mOwner -> GetGame() -> GetRenderer() -> RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader)
{
    if (mTexture)
    {
        // scale the quad by the width/height of texture
        Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(mTexWidth),
                                                static_cast<float>(mTexHeight),
                                                1.0f);
        
        Matrix4 world = scaleMat * mOwner -> GetWorldTransform();
        
        // since all sprites use the same shader/vertices
        // the game first sets them active before any sprite draws
        
        // set world transform
        shader -> SetMatrixUniform("uWorldTransform", world);
        
        // set the current texture
        mTexture -> SetActive();
        
        // draw quad
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void SpriteComponent::SetTexture(Texture* texture)
{
    mTexture = texture;
    
    // set width/height
    mTexWidth = texture -> GetWidth();
    mTexHeight = texture -> GetHeight();
}
