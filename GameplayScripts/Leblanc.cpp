#pragma once
#include <stdio.h>
#include "Leblanc.h"
#include "../Input.h"
#include "../MemSnapshot.h"
#include "../Utils.h"


void Leblanc::Prior1() {
    if (Input::IsKeyDown(HKey::C)) {

    }

    if (Input::IsKeyDown(HKey::V)) {

    }
}

void Leblanc::Prior2() {
    if (Input::IsKeyDown(HKey::SPACE)) {

    }

    if (Input::IsKeyDown(HKey::V)) {

    }
}

void Leblanc::Prior3(){
    if (Input::IsKeyDown(HKey::SPACE)) {

    }
}

void Leblanc::Draw() {
    for (Vector3 p : gameapi.ms->player->posLst) {
        gameapi.DrawCircleWorld(p, 10, 10, 3, ImVec4(0, 255, 0, 100));
    }
    std::vector<TurnPoint> turnPoints = gameapi.GetTurningPoint(gameapi.ms->player);
    for (TurnPoint tp : turnPoints) {
        gameapi.DrawCircleWorld(tp.pos, 10, 10, 3, ImVec4(0, 0, 255, 100));
    }



}
