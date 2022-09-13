#pragma once
#include <stdio.h>
#include "Orbwalk.h"
#include "../Input.h"
#include "../MemSnapshot.h"
#include "../Utils.h"

#define FULL_HP 60
#define HP_OFFSET -32

void Orbwalk::Prior1() {
    if (Input::IsKeyDown(HKey::C)) {
        if (CanAttack()) {
            float range = GetAARange();
            std::shared_ptr<GameObject> minion = Lasthitable(range);
            if (minion != NULL)
                Attack(minion);
        }
    }

    if (Input::IsKeyDown(HKey::V)) {
        if (CanAttack()) {
            float range = GetAARange();
            std::shared_ptr<GameObject> minion = Lasthitable(range);
            if (minion != NULL)
                Attack(minion);
        }
        if (!CanMove() &&  MissileCheck()) {
            std::pair <float, float> calc = CalcAttTime();
            float att_time = calc.first;
            float windup_time = calc.second;

            long att_delay = (long)((att_time-windup_time) * 1000) + last_delay;
            attTick = high_resolution_clock::now() + milliseconds(att_delay);
            movTick = high_resolution_clock::now();
            //attTick = gameapi.ms->gameTime + att_time - windup_time + last_delay;
            //movTick = gameapi.ms->gameTime;
        }
    }
}

void Orbwalk::Prior2() {
    if (Input::IsKeyDown(HKey::SPACE) && CanAttack()) {
        float range = GetAARange();
        target = gameapi.FindEnemy(range);
        if (target != NULL) {
            Attack(target);
        }
    }

    if (Input::IsKeyDown(HKey::V) && CanAttack()) {
        float range = GetAARange();
        std::shared_ptr<GameObject> minion = gameapi.FindEnemyMinion(range);
        if (minion != NULL && !ShouldWait(minion)) {
            Attack(minion);
        }
    }
}

void Orbwalk::Prior3(){
    if ( (Input::IsKeyDown(HKey::V)||Input::IsKeyDown(HKey::C)||Input::IsKeyDown(HKey::SPACE)) && 
            CanMove() && CanClick()) {
        Input::KeyDown(TILDE);
        Input::PressRightClick();
        Input::KeyUp(TILDE);
        cliTick = high_resolution_clock::now() + milliseconds(100);
    }
}

void Orbwalk::Draw() {
    float range = GetAARange();

    // Draw aa range
    gameapi.DrawCircleWorld(gameapi.ms->player->position, range, 30, 3, ImVec4(255, 0, 0, 100));

    // Draw last hit hp bar
    float att = GetAADmg();
    std::vector<std::shared_ptr<GameObject>> missiles = GetMissiles();
    std::vector<std::shared_ptr<GameObject>> minions;
    gameapi.GetEnemyMinions(1000, minions);
    for (std::shared_ptr<GameObject> minion : minions){
        float dmg = League::EffectiveDamage(att, minion->armour);
        float predHealth = PredHealth(missiles, minion);
        bool willDie = dmg > (minion->health - predHealth);
        Vector2 p = gameapi.HpBarPos(minion);
        float lasthit = dmg/minion->maxHealth * FULL_HP;
        Vector2 start = Vector2(p.x + HP_OFFSET + lasthit, p.y - 8);
        Vector2 end   = Vector2(p.x + HP_OFFSET + lasthit, p.y - 2);
        gameapi.DrawLine(start, end, 1,ImVec4(255, 255, 255, 100));
        if (willDie) {
            Vector4 box = Vector4(p.x + HP_OFFSET, p.y - 8, p.x + HP_OFFSET + FULL_HP, p.y - 2);
            gameapi.DrawRect(box,ImVec4(255, 255, 255, 100), 0, 1);
        }
    }
}

float Orbwalk::GetAARange() {
    std::shared_ptr<GameObject> myHero = gameapi.ms->player;
    float range = myHero->GetAttackRange();
    if (myHero->name.compare("jinx") == 0 && myHero->HasBuff("JinxQ")) {
        float qRange[] = {0,100,125,150,175,200};
        range += qRange[myHero->Q.level];
    }
    return range;
}

float Orbwalk::GetAADmg() {
    std::shared_ptr<GameObject> myHero = gameapi.ms->player;
    float att = myHero->GetBasicAttackDamage();
    return att;
}

float Orbwalk::GetAAMissileSpeed() {
    if (gameapi.ms->player->name.compare("jinx") == 0 && gameapi.ms->player->HasBuff("JinxQ")) {
        return 2000.f;
    }
    return gameapi.ms->player->GetBasicAttackMissileSpeed();
}

bool Orbwalk::MissileCheck() {
    for (std::shared_ptr<GameObject> missile : (gameapi.ms->missiles)){
        bool hasAtt = missile->name.find("attack")!= std::string::npos;
        bool hasName = missile->name.find(gameapi.ms->player->name.c_str())!= std::string::npos;

        if (hasAtt && hasName) {
            return true;
        }
    }
    return false;
}

std::shared_ptr<GameObject> Orbwalk::Lasthitable(float range){
    float att = GetAADmg();
    std::vector<std::shared_ptr<GameObject>> missiles = GetMissiles();

    std::shared_ptr<GameObject> minion = gameapi.FindEnemyMinion(range+5);
    if (minion == NULL)
        return NULL;
    float dmg = League::EffectiveDamage(att, minion->armour);
    float predHealth = PredHealth(missiles, minion);
    if (dmg > (minion->health - predHealth)) {
        return minion;
    }

    return NULL;
}

float Orbwalk::PredHealth(std::vector<std::shared_ptr<GameObject>> &missiles, 
    std::shared_ptr<GameObject> minion) {
    float health = 0;
    std::pair <float, float> calc = CalcAttTime();
    float windup_time = calc.second;
    float my_misl_time = gameapi.Distance(gameapi.ms->player,minion)/GetAAMissileSpeed()
            + gameapi.ms->player->GetBasicAttackWindup();
    for (std::shared_ptr<GameObject> missile : missiles){
        DWORD destNetID = gameapi.ms->indexToNetId[missile->destIndex];
        if (destNetID != minion->networkId)
            continue;
        GameObject* allyMinion = gameapi.GetObjectByNetId(destNetID);
        float misl_time = (gameapi.Distance(missile,minion) - minion->GetGameplayRadius()) / allyMinion->GetBasicAttackMissileSpeed();
        if (misl_time < my_misl_time) {
            float basicDmg = allyMinion->GetBasicAttackDamage();
            float mislDmg  = League::EffectiveDamage(basicDmg, minion->armour);
            health += mislDmg;
        }
    }
    return health;
}

std::vector<std::shared_ptr<GameObject>> Orbwalk::GetMissiles() {
    std::vector<std::shared_ptr<GameObject>> missiles;
    for (std::shared_ptr<GameObject> missile : (gameapi.ms->missiles)){
        bool hasAtt  = missile->name.find("attack")!= std::string::npos;
        bool hasName = missile->name.find("minion")!= std::string::npos;
        bool isAlly  = missile->IsAllyTo(gameapi.ms->player);
        bool isInRange = gameapi.DistanceSqr(gameapi.ms->player, missile) < (900*900);

        if (hasAtt && hasName && isAlly && isInRange){
            missiles.push_back(missile);
        }
    }
    return missiles;
}


bool Orbwalk::ShouldWait(std::shared_ptr<GameObject> minion) {
    float att = GetAADmg();
    float dmg = League::EffectiveDamage(att,minion->armour);

    //std::vector<std::shared_ptr<GameObject>> missiles = GetMissiles();
    //float predHP = PredHealth(missiles, minion);
    //if (predHP == 0) {
    //    return false;
    //}

    std::vector<std::shared_ptr<GameObject>> allyMinions;
    gameapi.GetAllyMinions(800, allyMinions);
    if (allyMinions.size() == 0) {
        return false;
    }
    float totalAllyAtt = 0;
    for (std::shared_ptr<GameObject> minion : allyMinions){
        totalAllyAtt += minion->GetBasicAttackDamage();
    }
    float allyDmg = League::EffectiveDamage(totalAllyAtt,minion->armour);

    //if (minion->health > dmg &&  minion->health  < 2.7*dmg) {
    if ( (minion->health-dmg) > 0 && (minion->health-dmg) < 0.8*allyDmg) {
        return true;
    }
    return false;
}

void Orbwalk::Attack(std::shared_ptr<GameObject> att_target){
    if (!CanClick()) {
        return;
    }
    cliTick = high_resolution_clock::now() + milliseconds(50);
    Vector2 pos = gameapi.WorldToScreen(att_target->position);
    Vector2 oldPos = Input::GetCursorPosition();
    Input::MoveCursorTo(pos.x, pos.y);
    Input::PressKey(HKey::BACKSLASH);
    Input::MoveCursorTo(oldPos.x, oldPos.y);

    std::pair <float, float> calc = CalcAttTime();
    float att_time = calc.first;
    float windup_time = calc.second;

    long att_delay = (long)(att_time * 1000) + last_delay;
    attTick = high_resolution_clock::now() + milliseconds(att_delay);
    //attTick = gameapi.ms->gameTime + att_time + last_delay;
    
    long mov_delay = (long)(windup_time * 1000) + last_delay;
    movTick = high_resolution_clock::now() + milliseconds(mov_delay);
    //movTick = gameapi.ms->gameTime + windup_time + last_delay;
}

std::pair <float, float> Orbwalk::CalcAttTime() {
    float att_speed = gameapi.ms->player->GetBaseAttackSpeed() * gameapi.ms->player->atkSpeedMulti;
    float att_time = 1.0 / att_speed;

    //float windup_time = (1.0 / gameapi.ms->player->GetBaseAttackSpeed()) * gameapi.ms->player->GetBasicAttackWindup();
    float windup_time = 1.0f/((att_speed + 1.0f) * 0.658f);
    
    return std::pair <float, float> (att_time, windup_time);
}

bool Orbwalk::CanMove(){
    return (high_resolution_clock::now() > movTick);
    //return (gameapi.ms->gameTime > movTick);
}

bool Orbwalk::CanAttack(){
    return (high_resolution_clock::now() > attTick);
    //return (gameapi.ms->gameTime > attTick);
}

bool Orbwalk::CanClick(){
    return (high_resolution_clock::now() > cliTick);
    //return (gameapi.ms->gameTime > cliTick);
}

bool Orbwalk::IsAttacking() {
    return (high_resolution_clock::now() > attackingTick);
}

bool Orbwalk::IsRClicking() {
    return (high_resolution_clock::now() > rClickingTick);
}
