#include "GameAPI.h"

void GameAPI::Init(MemSnapshot* init_ms) {
    ms = init_ms;
}

Vector2 GameAPI::WorldToScreen(const Vector3& pos) {
    return ms->renderer->WorldToScreen(pos);
}

Vector2 GameAPI::WorldToMinimap(const Vector3& pos) {
    return ms->renderer->WorldToMinimap(pos, ms->minimapPos, ms->minimapSize);
}

float GameAPI::DistanceToMinimap(float dist) {
    return ms->renderer->DistanceToMinimap(dist, ms->minimapSize);
}

float GameAPI::Distance(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second) {
    //int key = (first->objectIndex > second->objectIndex) ?
    //    (first->objectIndex << 16) | second->objectIndex : 
    //    (second->objectIndex << 16) | first->objectIndex;

    //auto it = distanceCache.find(key);
    //if (it != distanceCache.end())
    //    return it->second;

    Vector2 p1 = { first->position.x, first->position.z };
    Vector2 p2 = { second->position.x, second->position.z };
    float dist = p1.distance(p2);

    //distanceCache[key] = dist;

    return dist;
}

float GameAPI::DistanceSqr(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second) {
    //int key = (first->objectIndex > second->objectIndex) ?
    //    (first->objectIndex << 16) | second->objectIndex : 
    //    (second->objectIndex << 16) | first->objectIndex;

    //auto it = distanceSqrCache.find(key);
    //if (it != distanceSqrCache.end())
    //    return it->second;

    Vector2 p1 = { first->position.x, first->position.z };
    Vector2 p2 = { second->position.x, second->position.z };
    float dist = p1.distance_sqr(p2);

    //distanceSqrCache[key] = dist;

    return dist;
}

bool GameAPI::IsScreenPointOnScreen(const Vector2& point, float offsetX = 0.f, float offsetY = 0.f) {
    return ms->renderer->IsScreenPointOnScreen(point, offsetX, offsetY);
}

bool GameAPI::IsWorldPointOnScreen(const Vector3& point, float offsetX = 0.f, float offsetY = 0.f) {
    return ms->renderer->IsWorldPointOnScreen(point, offsetX, offsetY);
}

void GameAPI::DrawCircle(const Vector2& center, float radius, int numPoints, float thickness, const ImVec4& color) {
    draw_lst->AddCircle(ImVec2(center.x, center.y), radius, ImColor(color), numPoints, thickness);
}

void GameAPI::DrawCircleFilled(const Vector2& center, float radius, int numPoints, const ImVec4& color) {
    draw_lst->AddCircleFilled(ImVec2(center.x, center.y), radius, ImColor(color), numPoints);
}

void GameAPI::DrawTxt(const Vector2& pos, const char* text, const ImVec4& color) {
    draw_lst->AddText(ImVec2(pos.x, pos.y), ImColor(color), text);
}

void GameAPI::DrawRect(const Vector4& box, const ImVec4& color, float rounding = 0, float thickness = 1.0) {
    draw_lst->AddRect(ImVec2(box.x, box.y), ImVec2(box.z, box.w), ImColor(color), rounding, 15, thickness);
}

void GameAPI::DrawRectFilled(const Vector4& box, const ImVec4& color, float rounding = 0) {
    draw_lst->AddRectFilled(ImVec2(box.x, box.y), ImVec2(box.z, box.w), ImColor(color), rounding);
}

void GameAPI::DrawRectWorld(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4, float thickness, const ImVec4& color) {
    static Vector2 points[4];
    points[0] = ms->renderer->WorldToScreen(p1);
    points[1] = ms->renderer->WorldToScreen(p2);
    points[2] = ms->renderer->WorldToScreen(p3);
    points[3] = ms->renderer->WorldToScreen(p4);

    draw_lst->AddPolyline((ImVec2*)points, 4, ImColor(color), true, thickness);
}

void GameAPI::DrawTriangleWorld(const Vector3& p1, const Vector3& p2, const Vector3& p3, float thickness, const ImVec4& color) {
    draw_lst->AddTriangle(
        (ImVec2&)ms->renderer->WorldToScreen(p1), 
        (ImVec2&)ms->renderer->WorldToScreen(p2),
        (ImVec2&)ms->renderer->WorldToScreen(p3), ImColor(color), thickness);
}

void GameAPI::DrawTriangleWorldFilled(const Vector3& p1, const Vector3& p2, const Vector3& p3, const ImVec4& color) {
    draw_lst->AddTriangleFilled(
        (ImVec2&)ms->renderer->WorldToScreen(p1), 
        (ImVec2&)ms->renderer->WorldToScreen(p2),
        (ImVec2&)ms->renderer->WorldToScreen(p3), ImColor(color));
}

void GameAPI::DrawCircleWorld (const Vector3& center, float radius, int numPoints, float thickness, const ImVec4& color) {
    ms->renderer->DrawCircleAt(draw_lst, center, radius, false, numPoints, ImColor(color), thickness);
}

void GameAPI::DrawCircleWorldFilled(const Vector3& center, float radius, int numPoints, const ImVec4& color) {
    ms->renderer->DrawCircleAt(draw_lst, center, radius, true, numPoints, ImColor(color));
}

void GameAPI::DrawLine(const Vector2& start, const Vector2& end, float thickness, const ImVec4& color) {
    draw_lst->AddLine((const ImVec2&)start, (const ImVec2&)end, ImColor(color), thickness);
}

void GameAPI::DrawLineWorld(const Vector3& start, const Vector3& end, float thickness, const ImVec4& color) {
    draw_lst->AddLine(
        (ImVec2&)ms->renderer->WorldToScreen(start), 
        (ImVec2&)ms->renderer->WorldToScreen(end), ImColor(color), thickness);
}

void GameAPI::DrawImage(const char* img, const Vector2& start, const Vector2& end, const ImVec4& color) {
    static ImVec2 zero = ImVec2(0.f, 0.f);
    static ImVec2 one = ImVec2(1.f, 1.f);

    auto it = GameData::Images.find(std::string(img));
    if (it == GameData::Images.end())
        return;
    draw_lst->AddImage(it->second->resourceView, (ImVec2&)start, (ImVec2&)end, zero, one, ImColor(color));
}

void GameAPI::DrawImageRounded(const char* img, const Vector2& start, const Vector2& end, const ImVec4& color, float rounding) {
    static ImVec2 zero = ImVec2(0.f, 0.f);
    static ImVec2 one = ImVec2(1.f, 1.f);

    auto it = GameData::Images.find(std::string(img));
    if (it == GameData::Images.end())
        return;
    draw_lst->AddImageRounded(it->second->resourceView, (ImVec2&)start, (ImVec2&)end, zero, one, ImColor(color), rounding);
    
}

void GameAPI::DrawButton(const Vector2& p, const char* text, ImVec4& colorButton, ImVec4& colorText, float rounding = 0) {
    int txtSize = strlen(text);
    draw_lst->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + txtSize * 7.2f + 5, p.y + 17), ImColor(colorButton), rounding);
    draw_lst->AddText(ImVec2(p.x + 5, p.y + 2), ImColor(colorText), text);
}

Vector2 GameAPI::HpBarPos(std::shared_ptr<GameObject> obj) {
    Vector3 pos = obj->position.clone();
    pos.y += obj->GetHpBarHeight();

    Vector2 w2s = ms->renderer->WorldToScreen(pos);
    w2s.y -= (ms->renderer->height * 0.00083333335f * obj->GetHpBarHeight());

    return w2s;
}

SpellInfo* GameAPI::GetSpellInfo(const char* spellName) {
    std::string name(spellName);
    return GameData::GetSpellInfoByName(name);
}

Vector2 GameAPI::LinearCollision(const Vector2& p1, const Vector2& d1, const Vector2& p2, const Vector2& d2, float radius) {
    static float Ax, Bx, Cx, Ay, By, Cy;
    static float a, b, c, delta;
    static float sqrt_d, t1, t2;

    Ax = pow((d1.x - d2.x), 2.f);
    Bx = p1.x*d1.x - p1.x*d2.x - p2.x*d1.x + p2.x*d2.x;
    Cx = pow((p1.x - p2.x), 2.f);

    Ay = pow((d1.y - d2.y), 2.f);
    By = p1.y*d1.y - p1.y*d2.y - p2.y*d1.y + p2.y*d2.y;
    Cy = pow((p1.y - p2.y), 2.f);

    a = Ax + Ay;
    b = 2.f*(Bx + By);
    c = Cx + Cy - pow(radius, 2.f);
    delta = b * b - 4.f*a*c;

    if (a == 0.f || delta < 0.f)
        return Vector2(-1.f, -1.f);

    sqrt_d = sqrt(delta);
    t1 = (-b + sqrt_d) / (2.f*a);
    t2 = (-b - sqrt_d) / (2.f*a);

    return Vector2(t1, t2);
}

GameObject *GameAPI::GetObjectByIndex(short index) {
    auto it = ms->indexToNetId.find(index);
    if (it == ms->indexToNetId.end())
        return nullptr;

    auto it2 = ms->objectMap.find(it->second);
    if (it2 == ms->objectMap.end())
        return nullptr;

    return it2->second.get();
}

GameObject *GameAPI::GetObjectByNetId(int net_id) {
    auto it = ms->objectMap.find(net_id);
    return (it != ms->objectMap.end()) ? it->second.get() : nullptr;
}


void GameAPI::GetAllyMinions(float range, std::vector<std::shared_ptr<GameObject>> &minions) {
    range += 5;
    for (std::shared_ptr<GameObject> minion : (ms->minions)) {
        float dist = DistanceSqr(ms->player, minion);
        bool isAlly = minion->IsAllyTo(ms->player);

        if (isAlly && dist < (range*range) && minion->isAlive) {
            minions.push_back(minion);
        }
    }
}

void GameAPI::GetEnemyMinions(float range, std::vector<std::shared_ptr<GameObject>> &minions) {
    range += 5;
    for (std::shared_ptr<GameObject> minion : (ms->minions)) {
        float dist = DistanceSqr(ms->player, minion);
        bool isEnemy = minion->IsEnemyTo(ms->player);

        if (isEnemy && dist < (range*range) && minion->isAlive) {
            minions.push_back(minion);
        }
    }

    if (minions.size() > 0)
        return;

    for (std::shared_ptr<GameObject> minion : (ms->jungle)) {
        float dist = DistanceSqr(ms->player, minion);

        if (dist < (range*range) && minion->isAlive) {
            minions.push_back(minion);
        }
    }
}

void GameAPI::GetEnemies(float range, std::vector<std::shared_ptr<GameObject>> &enemies) {
    range += 5;

    for (std::shared_ptr<GameObject> champ : (ms->champions)) {
        float dist = DistanceSqr(ms->player, champ);
        bool isEnemy = champ->IsEnemyTo(ms->player);
        bool isClone = champ->R.name.compare(champ->D.name) == 0;

        if (isEnemy && dist < (range*range) && champ->isAlive && !isClone) {
            enemies.push_back(champ);
        }
    }
}

std::shared_ptr<GameObject> GameAPI::FindEnemyMinion(float range) {
    float minHealth = 999999;
    std::shared_ptr<GameObject> sel_minion = NULL;
    std::vector<std::shared_ptr<GameObject>> minions;
    GetEnemyMinions(range + 5, minions);
    for (std::shared_ptr<GameObject> minion : minions) {
        if (minion->health < minHealth) {
            sel_minion = minion;
            minHealth = minion->health;
        }
    }
    return sel_minion;
}


std::shared_ptr<GameObject> GameAPI::FindEnemy(float range) {
    float minHealth = 999999;
    std::shared_ptr<GameObject> sel_enemy = NULL;
    std::vector<std::shared_ptr<GameObject>> enemies;
    GetEnemies(range + 5, enemies);
    for (std::shared_ptr<GameObject> enemy : enemies) {
        if (enemy->isVisible && enemy->health < minHealth) {
            sel_enemy = enemy;
            minHealth = enemy->health;
        }
    }
    return sel_enemy;
}

bool GameAPI::IsInRange(Vector3& first, Vector3& second, float range) {
    Vector2 p1 = { first.x, first.z };
    Vector2 p2 = { second.x, second.z };
    float dist = p1.distance_sqr(p2);
    return dist < pow(range,2);
}

bool GameAPI::IsInRange2(Vector2& first, Vector2& second, float range) {
    float dist = first.distance_sqr(second);
    return dist < pow(range,2);
}

bool IsTurningPoint(Vector3 &p1, Vector3 &p2, Vector3 &p3) {
    float ax = p1.x - p2.x;
    float ay = p1.z - p2.z;
    float bx = p3.x - p2.x;
    float by = p3.z - p2.z;

    float dot  = (ax * bx + ay * by);
    float lenSqr = (ax * ax + ay * ay)*(bx * bx + by * by);
    float cosSqr = pow(dot, 2) / lenSqr;
    constexpr auto cosSqr10Deg = 0.96984631039;
    if (dot < 0 && cosSqr > cosSqr10Deg) {
        return false;
    }
    return true;
}

float GetAngleCos(Vector3 &p1, Vector3 &p2, Vector3 &p3) {
    float ax = p1.x - p2.x;
    float ay = p1.z - p2.z;
    float bx = p3.x - p2.x;
    float by = p3.z - p2.z;

    float dot = (ax * bx + ay * by);
    float len = sqrt((ax * ax + ay * ay)*(bx * bx + by * by));
    float cosx = dot / len;
    return cosx;
}

bool isSame(std::list<Vector3>::iterator p1, std::list<Vector3>::iterator p2) {
    return p1->x==p2->x && p1->z==p2->z;
}

std::list<Vector3>::iterator NextNotDup(std::list<Vector3>::iterator cur) {
    std::list<Vector3>::iterator prev = cur;
    cur++;
    while (isSame(cur, prev)) {
        prev = cur;
        cur++;
    }
    return cur;
}

std::vector<TurnPoint> GameAPI::GetTurningPoint(std::shared_ptr<GameObject> &target) {
    std::vector<TurnPoint> turnLst = std::vector<TurnPoint>();
    std::list<Vector3>& lst = target->posLst;
    std::list<Vector3>::iterator it;

    std::list<Vector3>::iterator p1 = lst.begin();
    std::list<Vector3>::iterator p2 = NextNotDup(p1);
    if (p2 == lst.end()) {
        return turnLst;
    }
    std::list<Vector3>::iterator p3 = NextNotDup(p2);
    while (p3!=lst.end()) {
        if (IsTurningPoint(*p1, *p2, *p3)) {
            float cosx = GetAngleCos(*p1, *p2, *p3);
            TurnPoint tp = {*p2, cosx};
            turnLst.push_back(tp);
        }
        p1 = p2;
        p2 = p3;
        p3 = NextNotDup(p2);
    }
    return turnLst;
}

void GameAPI::CastSpell(HKey key, Vector3 position) {
    if (!IsWorldPointOnScreen(position, 0, 0)) {
        return;
    }
    if (CanInput()) {
        Vector2 pos = WorldToScreen(position);
        Vector2 oldPos = Input::GetCursorPosition();
        MoveCursor(pos);
        PressKey(key);
        //MoveCursor(oldPos);
        inputTick = high_resolution_clock::now() + milliseconds(10);
    }
}

void GameAPI::MoveCursor(Vector2 pos) {
    Vector2 curPos = Input::GetCursorPosition();
    Input::MoveCursorTo(pos.x, pos.y);
    //while (!IsInRange2(curPos, pos, 5)) {
    //    Input::MoveCursorTo(pos.x, pos.y);
    //    curPos = Input::GetCursorPosition();
    //}
    inputTick = high_resolution_clock::now() + milliseconds(10);
}

bool GameAPI::PressKey(HKey key) {
    if (!block_input) {
        Input::KeyDown(key);
        //while (!Input::IsKeyDown(key)) {
        //    Input::KeyDown(key);
        //}
        time_point<high_resolution_clock> relTime = high_resolution_clock::now() + milliseconds(20);
        keysToRelease.push_back(std::pair<HKey,time_point<high_resolution_clock>>(key,relTime));
        return true;
    }
    return false;
}

void GameAPI::ReleaseKeys() {
    time_point<high_resolution_clock> curTime = high_resolution_clock::now();
    std::list<std::pair<HKey,time_point<high_resolution_clock>>>::iterator it;
    std::list<std::pair<HKey,time_point<high_resolution_clock>>>::iterator prevIt;
    it = keysToRelease.begin();
    while (it != keysToRelease.end()) {
        if ((*it).second < curTime) {
            Input::KeyUp((*it).first);
            //while (!Input::IsKeyUp((*it).first)) {
            //    Input::KeyUp((*it).first);
            //}
            prevIt = it;
            it++;
            keysToRelease.remove(*prevIt);
        } else {
            it++;
        }
    }
}

bool GameAPI::CanInput(){
    return (high_resolution_clock::now() > inputTick);
}

bool GameAPI::CanMouseMove(){
    return (high_resolution_clock::now() > mouseMoveTick);
}

Vector3 GameAPI::PredictPos(Vector3 &src, std::shared_ptr<GameObject> &dst, float delay, float speed, float width, float range) {
    bool foundPos = false;
    float xx = 0;
    float yy = 0;

    float ax = src.x;
    float ay = src.z;

    float bx = dst->position.x;
    float by = dst->position.z;

    float cx = dst->previousPosition.x;
    float cy = dst->previousPosition.z;

    if (bx==cx && by==cy){
        return dst->position;
    }

    float bc = dst->previousPosition.distance(dst->position);
    float vx = ( bx - cx ) / bc;
    float vy = ( by - cy ) / bc;

    float bv = dst->movementSpeed;
    float ab = src.distance(dst->position);

    if (speed > 1.f) {
        float hx = bx + ( vx * bv * ( delay + ab / speed ) );
        float hy = by + ( vy * bv * ( delay + ab / speed ) );
        float ah = sqrt((hx-ax)*(hx-ax)+(hy-ay)*(hy-ay));

        float ix = bx + ( vx * bv * ( delay + ah / speed ) );
        float iy = by + ( vy * bv * ( delay + ah / speed ) );
        float ai = sqrt((ix-ax)*(ix-ax)+(iy-ay)*(iy-ay));

        xx = bx + ( vx * bv * ( delay + ai / speed ) );
        yy = by + ( vy * bv * ( delay + ai / speed ) );

        float aj = sqrt((ax-xx)*(ax-xx)+(ay-yy)*(ay-yy));
        float bj = sqrt((bx-xx)*(bx-xx)+(by-yy)*(by-yy));

        float at = delay + aj / speed;
        float bt = bj / bv;
        foundPos = true;
        if (abs(at-bt) < 0.05f) {
            foundPos = true;
        }
    } else {
        xx = bx + ( vx * bv * delay );
        yy = by + ( vy * bv * delay );
        foundPos = true;
    }
    if (!foundPos){
        return Vector3(0,0,0);
    }
    float x = xx - ( vx * ( width / 2.f ) );
    float y = yy - ( vy * ( width / 2.f ) );
    float ap = pow(ax-x,2)+pow(ay-y,2);
    if (ap > pow(range,2)) {
        return Vector3(0,0,0);
    }
    return Vector3(x,0,y);
}

bool isCol(Vector3 &src, Vector3 &dst, float width, std::shared_ptr<GameObject> &minion) {
    float sx = src.x;
    float sy = src.z;

    float ex = dst.x;
    float ey = dst.z;

    float ux = minion->position.x;
    float uy = minion->position.z;

    float ubbox =  minion->GetGameplayRadius() + 25.f;
    float r2 = pow(width/2 + ubbox/2,2);

    float eu2 = pow(ex-ux,2) + pow(ey-uy,2);
    float su2 = pow(sx-ux,2) + pow(sy-uy,2);
    float se2 = pow(sx-ex,2) + pow(sy-ey,2);

    if (eu2>se2 || su2>se2) {
        return false;
    }
    float d2 = pow((ey-sy)*(sx-ux)+(ex-sx)*(uy-sy ), 2) / (pow(ey-sy,2) + pow(ex-sx,2));
    if (r2 > d2) {
        return true;
    }
    return false;
}

bool GameAPI::IsCollision(Vector3 &src, Vector3 &dst, float width,
        std::vector<std::shared_ptr<GameObject>> &Objects) {
    for (std::shared_ptr<GameObject> obj : Objects) {
        if (isCol(src, dst, width, obj)) {
            return true;
        }
    }
    return false;
}

void GameAPI::NumCollision(Vector3 &src, Vector3 &dst, float width, 
        std::vector<std::shared_ptr<GameObject>> &Objects, 
        std::vector<std::shared_ptr<GameObject>> &retLst) {
    for (std::shared_ptr<GameObject> obj : Objects) {
        if (isCol(src, dst, width, obj)) {
            retLst.push_back(obj);
        }
    }
}

int GameAPI::GetHitChance(Vector3 &src, Vector3 &dst, float delay, float speed, float width, float range, 
            std::shared_ptr<GameObject> &target) {
    int hitchance = 0;
    std::vector<TurnPoint> turnPoints = GetTurningPoint(target);
    if (turnPoints.size() == 0) {
        return 100;
    }

    return hitchance;
}
