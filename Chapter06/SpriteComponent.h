//
//  SpriteComponent.h
//  Game-mac
//
//  Created by Vincent Du on 2/25/20.
//  Copyright © 2020 Sanjay Madhav. All rights reserved.
//

#pragma once
#include "Component.h"
#include "SDL/SDL.h"
class SpriteComponent : public Component
{
public:
    // (Lower draw order corresponds with further back)
    SpriteComponent(class Actor* owner, int drawOrder = 100);
    ~SpriteComponent();

    virtual void Draw(class Shader* shader);
    virtual void SetTexture(class Texture* texture);

    int GetDrawOrder() const { return mDrawOrder; }
    int GetTexHeight() const { return mTexHeight; }
    int GetTexWidth() const { return mTexWidth; }
protected:
    class Texture* mTexture;
    int mDrawOrder;
    int mTexWidth;
    int mTexHeight;
};
