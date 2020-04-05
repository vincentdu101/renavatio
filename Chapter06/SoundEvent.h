//
//  SoundEvent.h
//  Game-mac
//
//  Created by Vincent Du on 4/3/20.
//  Copyright © 2020 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <string>
#include "Math.h"

class SoundEvent
{
public:
    SoundEvent();
    // returns true if associated FMOD event still exists
    bool IsValid();
    // restarts event from beginning
    void Restart();
    // stop this event
    void Stop(bool allowFadeOut = true);
    // setters
    void SetPaused(bool pause);
    void SetVolume(float value);
    void SetPitch(float value);
    void SetParameter(const std::string& name, float value);
    // getters
    bool GetPaused() const;
    float GetVolume() const;
    float GetPitch() const;
    float GetParameter(const std::string& name);
    // positional
    bool Is3D() const;
    void Set3DAttributes(const Matrix4& worldTrans);
protected:
    // make this constructor protected and audiosystem a friend
    // so that only AudioSystem can access this constructor
    friend class AudioSystem;
    SoundEvent(class AudioSystem* system, unsigned int id);
private:
    class AudioSystem* mSystem;
    unsigned int mID;
};
