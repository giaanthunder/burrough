#pragma once
#include <string>
#include <chrono>
#include "../MenuConfig.h"
#include "../GameAPI.h"
#include "../GameObject.h"

using namespace std::chrono;

class Leblanc {
public:
    Leblanc(GameAPI &api, MenuConfig &config) :gameapi(api), menuconfig(config) {}
    void Prior1();
    void Prior2();
    void Prior3();
    void Draw();

    bool enabled = true;


private:
    GameAPI &gameapi;
    MenuConfig &menuconfig;
};