//
//  AudioComponent.h
//  Game-mac
//
//  Created by Vincent Du on 3/28/20.
//  Copyright © 2020 Sanjay Madhav. All rights reserved.
//

#pragma once
#include "Component.h"
#include "SoundEvent.h"
#include <vector>
#include <string>

class AudioComponent : public Component
{
public:
    AudioComponent(class Actor* owner, int updateOrder = 200);
    ~AudioComponent();
    
    void Update(float deltaTime) override;
    void OnUpdateWorldTransform() override;
    
    SoundEvent PlayEvent(const std::string& name);
    void StopAllEvents();
private:
    std::vector<SoundEvent> mEvents2D;
    std::vector<SoundEvent> mEvents3D;
};
