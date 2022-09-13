#pragma once
#include "MemSnapshot.h"
#include "Utils.h"
#include <cmath>

//enum HitChance {
//    Impossible = 0, Low = 1, Medium = 2, High = 3
//};

struct TurnPoint {
    Vector3 pos;
    float cosx;
};


class GameAPI {
public:
    MemSnapshot*          ms;
    ImDrawList*           draw_lst;
    bool                  block_input=false;
    std::list<std::pair<HKey,time_point<high_resolution_clock>>> keysToRelease;
    time_point<high_resolution_clock> inputTick;
    time_point<high_resolution_clock> mouseMoveTick;

	void Init(MemSnapshot* ms);

    Vector2 WorldToScreen(const Vector3& pos);
    Vector2 WorldToMinimap(const Vector3& pos);

    float DistanceToMinimap(float dist);
    float Distance(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second);
    float DistanceSqr(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second);
    bool IsScreenPointOnScreen(const Vector2& point, float offsetX, float offsetY);
    bool IsWorldPointOnScreen(const Vector3& point, float offsetX, float offsetY);
    void DrawCircle(const Vector2& center, float radius, int numPoints, float thickness, const ImVec4& color);
    void DrawCircleFilled(const Vector2& center, float radius, int numPoints, const ImVec4& color);
    void DrawTxt(const Vector2& pos, const char* text, const ImVec4& color);
    void DrawRect(const Vector4& box, const ImVec4& color, float rounding, float thickness);
    void DrawRectFilled(const Vector4& box, const ImVec4& color, float rounding);
    void DrawRectWorld(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4, float thickness, const ImVec4& color);
    void DrawTriangleWorld(const Vector3& p1, const Vector3& p2, const Vector3& p3, float thickness, const ImVec4& color);
    void DrawTriangleWorldFilled(const Vector3& p1, const Vector3& p2, const Vector3& p3, const ImVec4& color);
    void DrawCircleWorld (const Vector3& center, float radius, int numPoints, float thickness, const ImVec4& color);
    void DrawCircleWorldFilled(const Vector3& center, float radius, int numPoints, const ImVec4& color);
    void DrawLine(const Vector2& start, const Vector2& end, float thickness, const ImVec4& color);
    void DrawLineWorld(const Vector3& start, const Vector3& end, float thickness, const ImVec4& color);
    void DrawImage(const char* img, const Vector2& start, const Vector2& end, const ImVec4& color);
    void DrawImageRounded(const char* img, const Vector2& start, const Vector2& end, const ImVec4& color, float rounding);
    void DrawButton(const Vector2& p, const char* text, ImVec4& colorButton, ImVec4& colorText, float rounding);

    Vector2 HpBarPos(std::shared_ptr<GameObject> obj);
    SpellInfo* GetSpellInfo(const char* spellName);
    Vector2 LinearCollision(const Vector2& p1, const Vector2& d1, const Vector2& p2, const Vector2& d2, float radius);
    GameObject *GetObjectByIndex(short index);
    GameObject *GetObjectByNetId(int net_id);

    void GetAllyMinions(float range, std::vector<std::shared_ptr<GameObject>> &minions);
    void GetEnemyMinions(float range, std::vector<std::shared_ptr<GameObject>> &minions);
    void GetEnemies(float range, std::vector<std::shared_ptr<GameObject>> &enemies);
    std::shared_ptr<GameObject> FindEnemyMinion(float range);
    std::shared_ptr<GameObject> FindEnemy(float range);

    bool IsInRange(Vector3& first, Vector3& second, float range);
    bool IsInRange2(Vector2& first, Vector2& second, float range);

    std::vector<TurnPoint> GetTurningPoint(std::shared_ptr<GameObject> &target);

    void CastSpell(HKey key, Vector3 pos);
    void MoveCursor(Vector2 pos);
    bool PressKey(HKey key);
    void ReleaseKeys();
    bool CanInput();
    bool CanMouseMove();

    Vector3 PredictPos(Vector3 &src, std::shared_ptr<GameObject> &dst, float delay, float speed, float width, float range);
    void NumCollision(Vector3 &src, Vector3 &dst, float width, 
            std::vector<std::shared_ptr<GameObject>> &Objects, 
            std::vector<std::shared_ptr<GameObject>> &retLst);
    bool IsCollision(Vector3 &src, Vector3 &dst, float width, std::vector<std::shared_ptr<GameObject>> &Objects);
    int GetHitChance(Vector3 &src, Vector3 &dst, float delay, float speed, float width, float range, 
            std::shared_ptr<GameObject> &target);
};


bool IsTurningPoint(Vector3 &p1, Vector3 &p2, Vector3 &p3);
float GetAngleCos(Vector3 &p1, Vector3 &p2, Vector3 &p3);
bool isSame(std::list<Vector3>::iterator p1, std::list<Vector3>::iterator p2);
std::list<Vector3>::iterator NextNotDup(std::list<Vector3>::iterator cur);
bool isCol(Vector3 &src, Vector3 &dst, float width, std::shared_ptr<GameObject> &minion);