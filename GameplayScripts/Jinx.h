#pragma once
#include <string>
#include <chrono>
#include "Orbwalk.h"
#include "../MenuConfig.h"
#include "../GameAPI.h"
#include "../GameObject.h"

using namespace std::chrono;

class Jinx {
public:
    Jinx(GameAPI &api, MenuConfig &config, Orbwalk* ow) :gameapi(api), menuconfig(config), orbwalk(ow) {}
    void Prior1();
    void Prior2();
    void Prior3();
    void Draw();

    bool enabled = true;
    bool condChecked = false;


private:
    GameAPI& gameapi;
    MenuConfig& menuconfig;
    Orbwalk* orbwalk;
};