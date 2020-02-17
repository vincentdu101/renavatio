//
//  PlaneActor.cpp
//  Game-mac
//
//  Created by Vincent Du on 2/16/20.
//  Copyright © 2020 Sanjay Madhav. All rights reserved.
//

#include "PlaneActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"

PlaneActor::PlaneActor(Game* game)
    :Actor(game)
{
    SetScale(10.0f);
    MeshComponent* mc = new MeshComponent(this);
    mc -> SetMesh(GetGame() -> GetRenderer() -> GetMesh("Assets/Plane.gpmesh"));
}
