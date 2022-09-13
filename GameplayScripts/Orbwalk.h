#pragma once
#include <string>
#include <chrono>
#include "../MenuConfig.h"
#include "../GameAPI.h"
#include "../GameObject.h"

using namespace std::chrono;

class Orbwalk {
public:
    Orbwalk(GameAPI &api, MenuConfig &config) :gameapi(api), menuconfig(config) {}
    void Prior1();
    void Prior2();
    void Prior3();
    void Draw();

    bool enabled = true;
    std::shared_ptr<GameObject> target;

    bool CanMove();
    bool CanAttack();
    bool CanClick();
    bool IsAttacking();
    bool IsRClicking();
    void Attack(std::shared_ptr<GameObject> att_target);

    std::vector<std::shared_ptr<GameObject>> missed_minion;


private:
    GameAPI &gameapi;
    MenuConfig &menuconfig;
    long last_delay = 100;
    time_point<high_resolution_clock>  attTick = high_resolution_clock::now();
    time_point<high_resolution_clock>  movTick = high_resolution_clock::now();
    time_point<high_resolution_clock>  cliTick = high_resolution_clock::now();
    time_point<high_resolution_clock>  attackingTick = high_resolution_clock::now();
    time_point<high_resolution_clock>  rClickingTick = high_resolution_clock::now();

    //float last_delay = 0.1;
    //float attTick = 0;
    //float movTick = 0;

    std::shared_ptr<GameObject> Lasthitable(float range);
    bool ShouldWait(std::shared_ptr<GameObject> minion);
    bool MissileCheck();
    std::vector<std::shared_ptr<GameObject>> GetMissiles();
    float PredHealth(std::vector<std::shared_ptr<GameObject>> &missiles, 
        std::shared_ptr<GameObject> minion);
    std::pair <float, float> CalcAttTime();
    float GetAARange();
    float GetAADmg();
    float GetAAMissileSpeed();
};