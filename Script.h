#pragma once
#include <chrono>
#include <string>
#include "MenuConfig.h"
#include "GameAPI.h"
#include "GameplayScripts/Orbwalk.h"
#include "GameplayScripts/Jinx.h"

using namespace std::chrono;

class Script {

public:
	void Init(GameAPI &api, MenuConfig &config);
    void Prior1();
    void Prior2();
    void Prior3();
	void Draw();

    duration<float, std::milli>  prior1TimeMs;
    duration<float, std::milli>  prior2TimeMs;
    duration<float, std::milli>  prior3TimeMs;
    duration<float, std::milli>  drawTimeMs;

    // Script list
    GameAPI *gameapi;
    Orbwalk *orbwalk; 
    Jinx *jinx; 

	~Script();
};