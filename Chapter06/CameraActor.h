//
//  CameraActor.h
//  Game-mac
//
//  Created by Vincent Du on 2/14/20.
//  Copyright © 2020 Sanjay Madhav. All rights reserved.
//

#pragma once
#include "Actor.h"

class CameraActor : public Actor
{
public:
    CameraActor(class Game* game);
    
    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t* keys) override;
private:
    class MoveComponent* mMoveComp;
};
