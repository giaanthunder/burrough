#include "GameObject.h"
#include "Utils.h"
#include "Offsets.h"
#include "Spell.h"
#include "GameData.h"

BYTE  GameObject::buff[GameObject::sizeBuff]         = {};
BYTE  GameObject::buffDeep[GameObject::sizeBuffDeep] = {};

GameObject::GameObject() {
    for (int i = 0; i < 10; i++) {
        Vector3 dump;
        posLst.push_back(dump);
    }
}

bool GameObject::HasUnitTags(const UnitTag& type1) const {
	return unitInfo->tags.test(type1);
}

bool GameObject::IsEqualTo(const std::shared_ptr<GameObject> other) const {
	return this->objectIndex == other->objectIndex;
}

bool GameObject::IsNotEqualTo(const std::shared_ptr<GameObject> other) const {
	return this->objectIndex != other->objectIndex;
}

float GameObject::GetAcquisitionRadius() const {
	return unitInfo->acquisitionRange;
}

float GameObject::GetSelectionRadius() const {
	return unitInfo->selectionRadius;
}

float GameObject::GetPathingRadius() const {
	return unitInfo->pathRadius;
}

float GameObject::GetGameplayRadius() const {
	return unitInfo->gameplayRadius;
}

float GameObject::GetBasicAttackMissileSpeed() const {
	return unitInfo->basicAttackMissileSpeed;
}

float GameObject::GetBasicAttackWindup() const {
	return unitInfo->basicAttackWindup;
}

float GameObject::GetAttackSpeedRatio() const {
	return unitInfo->attackSpeedRatio;
}

float GameObject::GetBaseMovementSpeed() const {
	return unitInfo->baseMovementSpeed;
}

float GameObject::GetBaseAttackSpeed() const {
	return unitInfo->baseAttackSpeed;
}

float GameObject::GetBaseAttackRange() const {
	return unitInfo->baseAttackRange;
}

float GameObject::GetAttackRange()  const {
	return GetBaseAttackRange() + GetGameplayRadius();
}

float GameObject::GetHpBarHeight() const {
	return unitInfo->healthBarHeight;
}

bool GameObject::IsEnemyTo(const std::shared_ptr<GameObject> other) const {
	return this->team != other->team;
}

bool GameObject::IsAllyTo(const std::shared_ptr<GameObject> other) const {
	return this->team == other->team;
}

void GameObject::LoadFromMem(DWORD base, HANDLE hProcess, bool deepLoad) {

	address = base;
	Mem::Read(hProcess, base, buff, sizeBuff);

	// Store previous position once N milliseconds to avoid the case when position == previousPosition but the object is moving
	std::chrono::duration<float, std::milli> timeDuration = high_resolution_clock::now() - timeSinceLastPreviousPosition;
	if (timeDuration.count() > 20) {
		previousPosition = position.clone();
		timeSinceLastPreviousPosition = high_resolution_clock::now();
	}

	memcpy(&team,          &buff[Offsets::ObjTeam],          sizeof(short));
	memcpy(&position,      &buff[Offsets::ObjPos],           sizeof(Vector3));
	memcpy(&health,        &buff[Offsets::ObjHealth],        sizeof(float));
	memcpy(&maxHealth,     &buff[Offsets::ObjMaxHealth],     sizeof(float));
    memcpy(&mana,          &buff[Offsets::ObjMana],          sizeof(float));
	memcpy(&baseAttack,    &buff[Offsets::ObjBaseAtk],       sizeof(float));
	memcpy(&bonusAttack,   &buff[Offsets::ObjBonusAtk],      sizeof(float));
	memcpy(&armour,        &buff[Offsets::ObjArmor],         sizeof(float));
	memcpy(&magicResist,   &buff[Offsets::ObjMagicRes],      sizeof(float));
	memcpy(&duration,      &buff[Offsets::ObjExpiry],        sizeof(float));
	memcpy(&isVisible,     &buff[Offsets::ObjVisibility],    sizeof(bool));
	memcpy(&objectIndex,   &buff[Offsets::ObjIndex],         sizeof(short));
	memcpy(&crit,          &buff[Offsets::ObjCrit],          sizeof(float));
	memcpy(&critMulti,     &buff[Offsets::ObjCritMulti],     sizeof(float));
	memcpy(&abilityPower,  &buff[Offsets::ObjAbilityPower],  sizeof(float));
	memcpy(&atkSpeedMulti, &buff[Offsets::ObjAtkSpeedMulti], sizeof(float));
	memcpy(&movementSpeed, &buff[Offsets::ObjMoveSpeed],     sizeof(float));
	memcpy(&networkId,     &buff[Offsets::ObjNetworkID],     sizeof(DWORD));

	// Check if alive
	DWORD spawnCount;
	memcpy(&spawnCount, &buff[Offsets::ObjSpawnCount], sizeof(int));
	isAlive = (spawnCount % 2 == 0);

	if (deepLoad) {

		char nameBuff[50];
		Mem::Read(hProcess, Mem::ReadDWORDFromBuffer(buff, Offsets::ObjName), nameBuff, 50);

		if (Character::ContainsOnlyASCII(nameBuff, 50))
			name = Character::ToLower(std::string(nameBuff));
		else
			name = std::string("");
		unitInfo = GameData::GetUnitInfoByName(name);
	}

	// Read extension of object
	if (HasUnitTags(Unit_Champion)) {
		LoadChampionFromMem(base, hProcess, deepLoad);
	}
	else if(unitInfo == GameData::UnknownUnit) {
		// Try reading missile extension
		LoadMissileFromMem(base, hProcess, deepLoad);
	}


}

// Champion stuff

DWORD GameObject::spellSlotPointerBuffer[6] = {};
BYTE  GameObject::itemListBuffer[0x100] = {};
BYTE  GameObject::buffListBuffer[0x100] = {};

void GameObject::LoadChampionFromMem(DWORD base, HANDLE hProcess, bool deepLoad) {

	// Read spells
	memcpy(&spellSlotPointerBuffer, &buff[Offsets::ObjSpellBook], sizeof(DWORD) * 6);

	Q.LoadFromMem(spellSlotPointerBuffer[0], hProcess);
	W.LoadFromMem(spellSlotPointerBuffer[1], hProcess);
	E.LoadFromMem(spellSlotPointerBuffer[2], hProcess);
	R.LoadFromMem(spellSlotPointerBuffer[3], hProcess);

	D.LoadFromMem(spellSlotPointerBuffer[4], hProcess);
	F.LoadFromMem(spellSlotPointerBuffer[5], hProcess);

	// Read items
	DWORD ptrList = Mem::ReadDWORD(hProcess, address + Offsets::ObjItemList);
	Mem::Read(hProcess, ptrList, itemListBuffer, 0x100);

	for (int i = 0; i < 6; ++i) {
		itemSlots[i].isEmpty = true;
		itemSlots[i].slot = i;

		DWORD itemPtr = 0, itemInfoPtr = 0;
		memcpy(&itemPtr, itemListBuffer + i * 0x10 + Offsets::ItemListItem, sizeof(DWORD));
		if (itemPtr == 0)
			continue;

		itemInfoPtr = Mem::ReadDWORD(hProcess, itemPtr + Offsets::ItemInfo);
		if (itemInfoPtr == 0)
			continue;
		
		int id = Mem::ReadDWORD(hProcess, itemInfoPtr + Offsets::ItemInfoId);
		itemSlots[i].isEmpty = false;
		itemSlots[i].stats = GameData::GetItemInfoById(id);
	}

	// Read level
	level = Mem::ReadDWORD(hProcess, base + Offsets::ObjLvl);


    // Read buff
    DWORD buffStartPtr = Mem::ReadDWORD(hProcess, base + Offsets::ObjBuffManager + Offsets::BuffStart);
    DWORD buffEndPtr   = Mem::ReadDWORD(hProcess, base + Offsets::ObjBuffManager + Offsets::BuffEnd);

    buffList.clear();
    for (DWORD BuffPtr = buffStartPtr; BuffPtr != buffEndPtr; BuffPtr += 0x8){
        DWORD buffEntry = Mem::ReadDWORD(hProcess, BuffPtr);
        buffEntry = Mem::ReadDWORD(hProcess, buffEntry + 0x8);
        Mem::Read(hProcess, buffEntry + Offsets::BuffName, buffListBuffer, 0x100);
        std::string buffName((char *)buffListBuffer);
        if (buffName.empty() ) {
            continue;
        }

        float startTime = Mem::ReadDWORD(hProcess, buffEntry + Offsets::BuffEntryBuffStartTime);
        float endTime   = Mem::ReadDWORD(hProcess, buffEntry + Offsets::BuffEntryBuffEndTime);
        DWORD c1 = Mem::ReadDWORD(hProcess, buffEntry + Offsets::BuffCount1);
        DWORD c2 = Mem::ReadDWORD(hProcess, buffEntry + Offsets::BuffCount2);
        DWORD c = (c2 - c1) >> 3;
        DWORD c3 = Mem::ReadDWORD(hProcess, buffEntry + 0x130);
        DWORD c4 = Mem::ReadDWORD(hProcess, buffEntry + 0x74);
        //printf("%s:\n %.2f -> %.2f : %lx - %lx - %lx - %lx\n", buffName.c_str(), startTime, endTime, c1, c2, c3, c4);

        std::shared_ptr<Buff> objBuff(new Buff());
        objBuff->name.assign((char *)buffListBuffer);
        objBuff->startTime = startTime;
        objBuff->endTime   = endTime;
        objBuff->countAlt  = c;
        objBuff->countF    = c3;
        objBuff->countI    = c4;
        buffList.push_back(objBuff);
    }

    // Save movement sequence
    std::chrono::duration<float, std::milli> timeDuration = high_resolution_clock::now() - timePosLst;
    if (timeDuration.count() > 50) {
        timePosLst = high_resolution_clock::now();
        posLst.pop_front();
        posLst.push_back(position);
    }
}

float GameObject::GetBasicAttackDamage() {
	return baseAttack + bonusAttack;
}

Spell* GameObject::GetSummonerSpell(SummonerSpellType type) {
	if (D.summonerSpellType == type)
		return &D;
	if (F.summonerSpellType == type)
		return &F;
	return nullptr;
}

bool GameObject::IsRanged() {
	return GetBaseAttackRange() >= 300.f;
}

// Missile stuff
void GameObject::LoadMissileFromMem(DWORD base, HANDLE hProcess, bool deepLoad) {

	if (!deepLoad)
		return;

	DWORD spellInfoPtr = Mem::ReadDWORDFromBuffer(buff, Offsets::MissileSpellInfo);
	if (spellInfoPtr == 0)
		return;

	DWORD spellDataPtr = Mem::ReadDWORD(hProcess, spellInfoPtr + Offsets::SpellInfoSpellData);
	if (spellDataPtr == 0)
		return;

	memcpy(&srcIndex,  buff + Offsets::MissileSrcIdx,   sizeof(short));
	memcpy(&destIndex, buff + Offsets::MissileDestIdx,  sizeof(short));
	memcpy(&startPos,  buff + Offsets::MissileStartPos, sizeof(Vector3));
	memcpy(&endPos,    buff + Offsets::MissileEndPos,   sizeof(Vector3));

	Mem::Read(hProcess, spellDataPtr, buff, 0x500);

	// Read name
	char nameBuff[50];
	Mem::Read(hProcess, Mem::ReadDWORD(hProcess, spellDataPtr + Offsets::SpellDataMissileName), nameBuff, 50);
	if (Character::ContainsOnlyASCII(nameBuff, 50))
		name = Character::ToLower(std::string(nameBuff));
	else
		name = std::string("");

	// Find static data
	spellInfo = GameData::GetSpellInfoByName(name);

	// Some spells require their end position to be projected using the range of the spell
	if (spellInfo != GameData::UnknownSpell && HasSpellFlags(ProjectedDestination)) {

		startPos.y += spellInfo->height;

		// Calculate direction vector and normalize
		endPos = Vector3(endPos.x - startPos.x, 0, endPos.z - startPos.z);
		endPos = endPos.normalize();

		// Update endposition using the height of the current position
		endPos.x = endPos.x*spellInfo->castRange + startPos.x;
		endPos.y = startPos.y;
		endPos.z = endPos.z*spellInfo->castRange + startPos.z;
	}
}

bool GameObject::EqualSpellFlags(SpellFlags flags) const {
	return spellInfo->flags == flags;
}

bool GameObject::HasSpellFlags(SpellFlags flags) const {
	return (spellInfo->flags & flags) == flags;
}

float GameObject::GetSpeed() const {
	return spellInfo->speed;
}

float GameObject::GetCastRange() const {
	return spellInfo->castRange;
}

float GameObject::GetWidth() const {
	return spellInfo->width;
}

float GameObject::GetCastRadius() const {
	return spellInfo->castRadius;
}

float GameObject::GetDelay() const {
	return spellInfo->delay;
}

float GameObject::GetHeight() const {
	return spellInfo->height;
}

float GameObject::GetTravelTime() const {
	return spellInfo->travelTime;
}

std::string GameObject::GetIcon() const {
	return spellInfo->icon;
}

bool GameObject::HasBuff(const char *name) const {
    for (std::shared_ptr<Buff> buff : buffList) {
        if (buff->name.compare(name) == 0){
            return true;
        }
    }
    return false;
}

