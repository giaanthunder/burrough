#include "GameData.h"
#include <fstream>
#include <filesystem>
#include "Utils.h"
#include "Overlay.h"
#include "external/rapidjson/document.h"

using namespace std::experimental;

UnitInfo*                         GameData::UnknownUnit  = new UnitInfo();
SpellInfo*                        GameData::UnknownSpell = new SpellInfo();
ItemInfo*                         GameData::UnknownItem  = new ItemInfo();
std::map<std::string, UnitInfo*>  GameData::Units        = {};
std::map<std::string, SpellInfo*> GameData::Spells       = {};
std::map<std::string, Texture2D*> GameData::Images       = {};
std::map<int, ItemInfo*>          GameData::Items        = {};

void GameData::Load(std::string& dataFolder) {
	std::string unitData         = dataFolder + "\\UnitData.json";
	std::string spellData        = dataFolder + "\\SpellData.json";
	std::string spellDataCustom  = dataFolder + "\\SpellDataCustom.json";
	std::string itemData         = dataFolder + "\\ItemData.json";
	std::string spellIcons       = dataFolder + "\\icons_spells";
	std::string champIcons       = dataFolder + "\\icons_champs";
	std::string extraIcons       = dataFolder + "\\icons_extra";

	printf("\r	Loading item data    \n");
	LoadItemData(itemData);

	printf("\r	Loading unit data    \n");
	LoadUnitData(unitData);

	printf("\r	Loading spell data   \n");
	LoadSpellData(spellData);
	LoadSpellData(spellDataCustom);

	printf("\r	Loading images      \n");
	LoadIcons(spellIcons);
	LoadIcons(champIcons);
	LoadIcons(extraIcons);

	printf("\r	Loading complete                             \n");
}

UnitInfo * GameData::GetUnitInfoByName(std::string& name)
{
	auto it = Units.find(name);
	if (it != Units.end())
		return it->second;
	return UnknownUnit;
}

SpellInfo * GameData::GetSpellInfoByName(std::string& name)
{
	auto it = Spells.find(name);
	if (it != Spells.end())
		return it->second;
	return UnknownSpell;
}

ItemInfo * GameData::GetItemInfoById(int id)
{
	auto it = Items.find(id);
	if (it != Items.end())
		return it->second;
	return UnknownItem;
}

void GameData::LoadUnitData(std::string&  path) {
	rapidjson::Document unitData = loadJSON(path);

	for (unsigned int i = 0; i < unitData.Size(); i++) {
		UnitInfo* unit = new UnitInfo();
		unit->acquisitionRange         = (float)unitData[i]["acquisitionRange"].GetDouble();
		unit->attackSpeedRatio         = (float)unitData[i]["attackSpeedRatio"].GetDouble();
		unit->baseAttackRange          = (float)unitData[i]["attackRange"].GetDouble();
		unit->baseAttackSpeed          = (float)unitData[i]["attackSpeed"].GetDouble();
		unit->baseMovementSpeed        = (float)unitData[i]["baseMoveSpeed"].GetDouble();
		unit->basicAttackMissileSpeed  = (float)unitData[i]["basicAtkMissileSpeed"].GetDouble();
		unit->basicAttackWindup        = (float)unitData[i]["basicAtkWindup"].GetDouble();
		unit->gameplayRadius           = (float)unitData[i]["gameplayRadius"].GetDouble();
		unit->healthBarHeight          = (float)unitData[i]["healthBarHeight"].GetDouble();
		unit->name                     = Character::ToLower(unitData[i]["name"].GetString());
		unit->pathRadius               = (float)unitData[i]["pathingRadius"].GetDouble();
		unit->selectionRadius          = (float)unitData[i]["selectionRadius"].GetDouble();

		rapidjson::Value::Array tags   = unitData[i]["tags"].GetArray();

		for (size_t j = 0; j < tags.Size(); j++)
			unit->SetTag(tags[j].GetString());

		Units[unit->name] = unit;
	}

}

void GameData::LoadSpellData(std::string& path) {
	rapidjson::Document spellData = loadJSON(path);

	for (unsigned int i = 0; i < spellData.Size(); i++) {
		SpellInfo* info = new SpellInfo();
		info->flags      = (SpellFlags)spellData[i]["flags"].GetInt();
		info->delay      = (float)spellData[i]["delay"].GetDouble();
		info->height     = (float)spellData[i]["height"].GetDouble();
		info->icon       = Character::ToLower(std::string(spellData[i]["icon"].GetString()));
		info->name       = Character::ToLower(std::string(spellData[i]["name"].GetString()));
		info->width      = (float)spellData[i]["width"].GetDouble();
		info->castRange  = (float)spellData[i]["castRange"].GetDouble();
		info->castRadius = (float)spellData[i]["castRadius"].GetDouble();
		info->speed      = (float)spellData[i]["speed"].GetDouble();
		info->travelTime = (float)spellData[i]["travelTime"].GetDouble();
		info->flags      = (SpellFlags)(info->flags | (spellData[i]["projectDestination"].GetBool() ? ProjectedDestination : 0));
		
		Spells[info->name] = info;
	}
}


void GameData::LoadItemData(std::string & path) {
	rapidjson::Document itemsData = loadJSON(path);

	for (unsigned int i = 0; i < itemsData.Size(); i++) {
		ItemInfo* info = new ItemInfo();
        info->movementSpeed        = (float)itemsData[i]["movementSpeed"].GetDouble();
        info->health               = (float)itemsData[i]["health"].GetDouble();
        info->crit                 = (float)itemsData[i]["crit"].GetDouble();
        info->abilityPower         = (float)itemsData[i]["abilityPower"].GetDouble();
        info->mana                 = (float)itemsData[i]["mana"].GetDouble();
        info->armour               = (float)itemsData[i]["armour"].GetDouble();
        info->magicResist          = (float)itemsData[i]["magicResist"].GetDouble();
        info->physicalDamage       = (float)itemsData[i]["physicalDamage"].GetDouble();
        info->attackSpeed          = (float)itemsData[i]["attackSpeed"].GetDouble();
        info->lifeSteal            = (float)itemsData[i]["lifeSteal"].GetDouble();
        info->hpRegen              = (float)itemsData[i]["hpRegen"].GetDouble();
        info->movementSpeedPercent = (float)itemsData[i]["movementSpeedPercent"].GetDouble();
        info->cost                 = (float)itemsData[i]["cost"].GetDouble();
        info->id                   = itemsData[i]["id"].GetInt();

		Items[info->id] = info;
	}
}

void GameData::LoadIcons(std::string& path) {
	std::string folder(path);
	WIN32_FIND_DATAA findData;
	HANDLE hFind;

	int nrFiles = std::distance(filesystem::directory_iterator(path), filesystem::directory_iterator());
	int nrFile = 0;
	hFind = FindFirstFileA((folder + "\\*.png").c_str(), &findData);
	do {
		if (hFind != INVALID_HANDLE_VALUE) {
			if (nrFile % 100 == 0)
				printf("\r	Loading %d/%d      ", nrFile, nrFiles);

			std::string filePath = folder + "/" + findData.cFileName;
			Texture2D* image = Texture2D::LoadFromFile(Overlay::GetDxDevice(), filePath);
			if (image == nullptr)
				printf("Failed to load: %s\n", filePath.c_str());
			else {
				std::string fileName(findData.cFileName);
				fileName.erase(fileName.find(".png"), 4);
				Images[Character::ToLower(fileName)] = image;
			}
		}
		nrFile++;
	} while (FindNextFileA(hFind, &findData));
}