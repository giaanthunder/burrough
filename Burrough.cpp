#define USE_IMPORT_EXPORT
#define USE_WINDOWS_DLL_SEMANTICS
#define STB_IMAGE_IMPLEMENTATION


#include <iostream>
#include <windows.h>
#include <map>
#include <list>
#include <conio.h>
#include <chrono>
#include <filesystem>

#include "LeagueMemoryReader.h"
#include "MapObject.h"
#include "GameData.h"
#include "GameAPI.h"
#include "Overlay.h"
#include "Script.h"
#include "Utils.h"

namespace fs = std::filesystem;

void MainLoop(Overlay& Overlay, LeagueMemoryReader& reader);

int main() {
    //printf("HELLO MASTER. I AM BURROUGH.");

    LeagueMemoryReader reader = LeagueMemoryReader();
    Overlay overlay = Overlay();

    std::string dataPath = fs::current_path().parent_path().string() + "\\data";
    std::string path;
	printf("[+] Initialising imgui and directx UI\n");
	overlay.Init();

    //try {

        printf("[+] Loading static map data\n\n");
        path = dataPath + "\\height_map_sru.bin";
        MapObject::Get(MapType::SUMMONERS_RIFT)->Load(path.c_str());
        path = dataPath + "\\height_map_ha.bin";
        MapObject::Get(MapType::HOWLING_ABYSS)->Load(path.c_str());

        printf("[+] Loading unit data\n");
        GameData::Load(dataPath);

        MainLoop(overlay, reader);

    //}
    //catch (std::runtime_error exception) {
    //    std::cout << exception.what() << std::endl;
    //}

    printf("Press any key to exit...");
    std::cin.get();
}

void MainLoop(Overlay& overlay, LeagueMemoryReader& reader) {
    MemSnapshot memSnapshot;
    GameAPI gameapi;
    MenuConfig menuconfig;
	Script script;
    std::string configPath = fs::current_path().parent_path().string() + "menu\\config.ini";
    bool rehook = true;

    unsigned int prior1PerSec = 1000;
    unsigned int prior2PerSec = 500;
    unsigned int prior3PerSec = 200;
    unsigned int drawPerSec   = 200;

    long prior1Interval = 1000000/prior1PerSec;
    long prior2Interval = 1000000/prior2PerSec;
    long prior3Interval = 1000000/prior3PerSec;
    long drawInterval   = 1000000/drawPerSec;

    high_resolution_clock::time_point prior1Tick;
    high_resolution_clock::time_point prior2Tick;
    high_resolution_clock::time_point prior3Tick;
    high_resolution_clock::time_point drawTick;


    printf("[i] Waiting for league process...\n");
    while (true) {
        if (reader.IsLeagueWindowActive()) {
            if (!overlay.IsVisible())
                overlay.Show();
        } else {
            if (overlay.IsVisible())
                overlay.Hide();
        }

        try {
            

            // Find the league process
            if (rehook) {
                Sleep(10);
                // Except if process not found
                reader.HookToProcess();
                printf("[i] Found league process. Hooked.\n");

                // First frame in game
                memSnapshot = MemSnapshot();
                menuconfig  = MenuConfig();
                gameapi.Init(&memSnapshot);
                script.Init(gameapi, menuconfig);
                rehook = false;
            } else {
                if (!reader.IsHookedToProcess()) {
                    printf("[i] Game ended. Waiting for new game...\n");
                    rehook = true;
                    continue;
                }

                // On tick
                reader.MakeSnapshot(memSnapshot);
                if (memSnapshot.gameTime < 2.f)
                    continue;

                script.Prior1();
                //printf("1");

                if (high_resolution_clock::now() > prior2Tick) {
                    prior2Tick = high_resolution_clock::now() + microseconds(prior2Interval);
                    script.Prior2();
                    //printf("2");
                }
                if (high_resolution_clock::now() > prior3Tick) {
                    prior3Tick = high_resolution_clock::now() + microseconds(prior3Interval);
                    script.Prior3();
                    //printf("3");
                }
                if (high_resolution_clock::now() > drawTick) {
                    drawTick = high_resolution_clock::now() + microseconds(drawInterval);
                    overlay.StartFrame();
                    gameapi.draw_lst = overlay.GetDrawList();
                    script.Draw();
                    //if (Input::IsKeyDown(HKey::LSHIFT))
                    //    overlay.DrawMenu(menuconfig);
                    overlay.RenderFrame();
                    //printf("D");
                }

            }
            Sleep(1);
        }
        catch (WinApiException exception) {
            // League process not found. Retry.
            rehook = true;
        }
        catch (std::runtime_error exception) {
            printf("[!] Unexpected error occured: \n [!] %s \n", exception.what());
            break;
        }
    }
}
