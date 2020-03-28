//
//  AudioComponent.cpp
//  Game-mac
//
//  Created by Vincent Du on 3/28/20.
//  Copyright © 2020 Sanjay Madhav. All rights reserved.
//

#include "AudioComponent.h"
#include "Actor.h"
#include "Game.h"
#include "AudioSystem.h"

AudioComponent::AudioComponent(Actor* owner, int updateOrder)
    :Component(owner, updateOwner)
{
}

AudioComponent::~AudioComponent()
{
    StopAllEvents();
}

void AudioComponent::Update(float deltaTime)
{
    Component::Update(deltaTime);
    
    // remove invalid 2D events
    auto iter = mEvents2D.begin();
    while (iter != mEvents2D.end()) {
        if (!iter -> IsValid())
        {
            iter = mEvents2D.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
    
    // remove invalid 3D events
    iter = mEvents3D.begin();
    while (iter != mEvents3D.end())
    {
        if (!iter -> IsValid())
        {
            if (!iter -> IsValid())
            {
                iter = mEvents3D.erase(iter);
            }
            else
            {
                ++iter;
            }
        }
    }
}

void AudioComponent::OnUpdateWorldTransform()
{
    // update 3D events' world transforms
    Matrix4 world = mOwner -> GetWorldTransform();
    for (auto& event : mEvents3D)
    {
        if (event.IsValid())
        {
            event.Set3DAttributes(world);
        }
    }
}

SoundEvent AudioComponent::PlayEvent(const std::string& name)
{
    SoundEvent e = mOwner -> GetGame() -> GetAudioSystem() -> PlayEvent(name);
    
    // is this 2d or 3d
    if (e.Is3D())
    {
        mEvents3D.emplace_back(e);
        // set initial 3D attributes
        e.Set3DAttributes(mOwner -> GetWorldTransform());
    }
    else
    {
        mEvents2D.emplace_back(e);
    }
    return e;
}

void AudioComponent::StopAllEvents()
{
    // stop all sounds
    for (auto& e : mEvents2D)
    {
        e.Stop();
    }
    for (auto& e : mEvents3D)
    {
        e.Stop();
    }
    // clear all events
    mEvents2D.clear();
    mEvents3D.clear();
}
