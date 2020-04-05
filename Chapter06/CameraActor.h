//
//  CameraActor.h
//  Game-mac
//
//  Created by Vincent Du on 2/14/20.
//  Copyright © 2020 Sanjay Madhav. All rights reserved.
//

#pragma once
#include "Actor.h"
#include "SoundEvent.h"

class CameraActor : public Actor
{
public:
    CameraActor(class Game* game);
    
    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t* keys) override;
    void SetFootstepSurface(float value);
private:
    class MoveComponent* mMoveComp;
    class AudioComponent* mAudioComp;
    SoundEvent mFootstep;
    float mLastFootstep;
};
