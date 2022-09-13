#include <stdio.h>
#include <string>
#include "Script.h"

void Script::Init(GameAPI &api, MenuConfig &config) {
    gameapi = &api;
	orbwalk = new Orbwalk(api, config);
    orbwalk->enabled = false;
	jinx = new Jinx(api, config, orbwalk);
}

void Script::Prior1() {
    high_resolution_clock::time_point beforeUpdate = high_resolution_clock::now();

    if (orbwalk->enabled)
        orbwalk->Prior1();
    if (jinx->enabled)
        jinx->Prior1();

    prior1TimeMs = high_resolution_clock::now() - beforeUpdate;
}

void Script::Prior2() {
    high_resolution_clock::time_point beforeUpdate = high_resolution_clock::now();

    if (orbwalk->enabled)
        orbwalk->Prior2();
    if (jinx->enabled)
        jinx->Prior2();

    prior2TimeMs = high_resolution_clock::now() - beforeUpdate;
}

void Script::Prior3() {
    high_resolution_clock::time_point beforeUpdate = high_resolution_clock::now();

    gameapi->ReleaseKeys();

    if (orbwalk->enabled)
        orbwalk->Prior3();
    if (jinx->enabled)
        jinx->Prior3();

    prior3TimeMs = high_resolution_clock::now() - beforeUpdate;
}

void Script::Draw() {
    high_resolution_clock::time_point beforeUpdate = high_resolution_clock::now();

    if (orbwalk->enabled)
        orbwalk->Draw();
    if (jinx->enabled)
        jinx->Draw();

    drawTimeMs = high_resolution_clock::now() - beforeUpdate;
}

Script::~Script() {
	delete orbwalk;
	delete jinx;
}
