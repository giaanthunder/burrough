#pragma once
#include <stdio.h>
#include "Jinx.h"
#include "../Input.h"
#include "../MemSnapshot.h"
#include "../Utils.h"


void Jinx::Prior1() {
    if (!condChecked) {
        if (gameapi.ms->player->name.compare("jinx") != 0) {
            enabled = false;
        } else {
            printf("Jinx detected!\n");
        }
        condChecked = true;
    }
    //if (orbwalk->enabled)
    //    orbwalk->Prior1();
    if (Input::IsKeyDown(HKey::C)) {

    }

    if (Input::IsKeyDown(HKey::V)) {

    }

    if (Input::IsKeyDown(HKey::SPACE)) {

    }
}

void Jinx::Prior2() {
    if (Input::IsKeyDown(HKey::SPACE)) {
        std::shared_ptr<GameObject> target;
        // Q skill
        Spell& qSpell = gameapi.ms->player->Q;
        float qRange[] = {0,100,125,150,175,200};
        float range1 = gameapi.ms->player->GetAttackRange();
        float range2 = range1 + qRange[qSpell.level];
        target = gameapi.FindEnemy(range2+5);
        if (target != NULL) {
            bool inRange1 = gameapi.IsInRange(gameapi.ms->player->position, target->position, range1);
            bool hasQ = gameapi.ms->player->HasBuff("JinxQ");
            // Q level < 3 is too weak
            if (qSpell.level < 3 && hasQ) { 
                Input::PressKey(HKey::Q);
            // Out range1 and Q is off => turn Q on
            } else if (!inRange1 && !hasQ && gameapi.ms->player->mana > 100) {
                Input::PressKey(HKey::Q);
            // In range1 and Q is on => turn Q off
            } else if (inRange1 && hasQ) { // 
                Input::PressKey(HKey::Q);
            }
        }

        // W skill
        Spell& wSpell = gameapi.ms->player->W;
        float wMana[] = {0., 50., 60., 70., 80., 90.};

        if (wSpell.GetRemainingCooldown(gameapi.ms->gameTime) > 0 || 
                gameapi.ms->player->mana < wMana[wSpell.level]) {
            return;
        }

        float range = 1450.;
        float delay = 0.25;
        float speed = 1200.;
        float width = 60.;

        target = gameapi.FindEnemy(range+200);
        Vector3& myPos  = gameapi.ms->player->position;
        if (target != NULL) {
            Vector3 pred = gameapi.PredictPos(myPos, target, delay, speed, width, range);
            std::vector<std::shared_ptr<GameObject>> colMinions;
            std::vector<std::shared_ptr<GameObject>> minions;
            gameapi.GetEnemyMinions(range+300, minions);
            bool isCol = gameapi.IsCollision(myPos, pred, width, minions);
            int hc = gameapi.GetHitChance(myPos, pred, delay, speed, width, range, target);
            if (!isCol && hc > 80) {
                gameapi.CastSpell(HKey::W, pred);
            }
        }

    }
    //if (orbwalk->enabled)
    //    orbwalk->Prior2();

    if (Input::IsKeyDown(HKey::V)) {
        Spell& qSpell = gameapi.ms->player->Q;
        if (gameapi.ms->player->HasBuff("JinxQ")) { 
            Input::PressKey(HKey::Q);
        }
    }
}

void Jinx::Prior3(){
    //if (orbwalk->enabled)
    //    orbwalk->Prior3();
    if (Input::IsKeyDown(HKey::SPACE)) {

    }
}

//std::vector<std::shared_ptr<Buff>> exist;
//bool isExist(std::shared_ptr<Buff> champ) {
//    for (auto c : exist) {
//        if (c == champ) {
//            return true;
//        }
//    }
//    return false;
//}

//std::vector<std::string> exist;
//bool isExist(std::string name) {
//    for (auto c : exist) {
//        if (c.compare(name)==0) {
//            return true;
//        }
//    }
//    return false;
//}

void drawTurnPoint(GameAPI& gameapi, std::shared_ptr<GameObject>& champ) {
    std::list<Vector3> lst = champ->posLst;
    std::list<Vector3>::iterator it = lst.begin();
    while (1) {
        Vector3 p1 = *it;
        it++;
        if (it == lst.end()) {
            break;
        }
        Vector3 p2 = *it;
        gameapi.DrawLineWorld(p1, p2, 2, ImVec4(0, 255, 0, 100));
    }
    std::vector<TurnPoint> turnPoints = gameapi.GetTurningPoint(champ);
    for (TurnPoint tp : turnPoints) {
        gameapi.DrawCircleWorld(tp.pos, 10, 10, 3, ImVec4(0, 0, 255, 100));
    }
}

bool printed = false;
void Jinx::Draw() {
    for (auto champ : gameapi.ms->champions) {
        if (champ->R.name.compare(champ->D.name) == 0) {
            continue;
        }
        drawTurnPoint(gameapi, champ);
    }
    //float range = gameapi.ms->player->W.GetCastRange();
    //float delay = gameapi.ms->player->W.GetDelay();
    //float speed = gameapi.ms->player->W.GetSpeed();
    //float width = gameapi.ms->player->W.GetWidth();
    float range = 1450.;
    float delay = 0.25;
    float speed = 1200.;
    float width = 60.;
    // if (!printed) {
    //     printf("range: %.2f, delay: %.2f, speed: %.2f, width %.2f\n",range, delay, speed, width);
    //     printed = true;
    // }

    //std::shared_ptr<GameObject> enemy = gameapi.FindEnemy(range);
    //Vector3& myPos  = gameapi.ms->player->position;
    //if (enemy != NULL) {
    //    Vector3 pred = gameapi.PredictPos(myPos, enemy, delay, speed, width, range);
    //    gameapi.DrawCircleWorld(pred, 30, 30, 3, ImVec4(255, 255, 255, 100));
    //    gameapi.DrawLineWorld(myPos, pred, 3, ImVec4(255, 255, 255, 100));
    //    std::vector<std::shared_ptr<GameObject>> colMinions;
    //    std::vector<std::shared_ptr<GameObject>> minions;
    //    gameapi.GetEnemyMinions(range+300, minions);
    //    gameapi.NumCollision(myPos, enemy->position, width, minions, colMinions);
    //    for (auto minion : colMinions) {
    //        gameapi.DrawCircleWorld(minion->position, minion->GetGameplayRadius(), 30, 3, ImVec4(255,0,0, 100));
    //    }
    //}


    //for (auto buff : gameapi.ms->player->buffList) {
    //    if (isExist(buff->name)) {
    //        continue;
    //    }
    //    exist.push_back(buff->name);
    //    printf("buff: %s\n",buff->name.c_str());
    //}

    //for (auto obj : gameapi.ms->others) {
    //    if (isExist(obj)) {
    //        continue;
    //    }
    //    if (!gameapi.IsInRange(gameapi.ms->player->position, obj->position, 2000)) {
    //        continue;
    //    }
    //    exist.push_back(obj);
    //    printf("object: %s\n",obj->name.c_str());
    //}
    //for (auto obj : gameapi.ms->missiles) {
    //    if (isExist(obj)) {
    //        continue;
    //    }
    //    if (!gameapi.IsInRange(gameapi.ms->player->position, obj->position, 2000)) {
    //        continue;
    //    }
    //    exist.push_back(obj);
    //    printf("missile: %s\n",obj->name.c_str());
    //}
}


