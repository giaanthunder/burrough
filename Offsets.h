#pragma once

/// Defines offsets for reading structs from league of legends memory
class Offsets {
	
public:
    Offsets() {};

    static int GameTime                        ;

    static int ObjIndex                        ;
    static int ObjTeam                         ;
    static int ObjNetworkID                    ;
    static int ObjPos                          ;
    static int ObjVisibility                   ;
    static int ObjSpawnCount                   ;
    static int ObjSrcIndex                     ;
    static int ObjMana                         ;
    static int ObjHealth                       ;
    static int ObjMaxHealth                    ;
    static int ObjArmor                        ;
    static int ObjMagicRes                     ;
    static int ObjBaseAtk                      ;
    static int ObjBonusAtk                     ;
    static int ObjMoveSpeed                    ;
    static int ObjSpellBook                    ;
    static int ObjName                         ;
    static int ObjLvl                          ;
    static int ObjExpiry                       ;
    static int ObjCrit                         ;
    static int ObjCritMulti                    ;
    static int ObjAbilityPower                 ;
    static int ObjAtkSpeedMulti                ;
    static int ObjAtkRange                     ;
    static int ObjRecallState                  ;
    static int ObjTransformation               ;
    static int ObjTargetable                   ;
    static int ObjInvulnerable                 ;
    static int ObjIsMoving                     ;
    static int ObjDirection                    ;

    static int ObjItemList                     ;
    static int ItemListItem                    ;
    static int ItemActiveName                  ;
    static int ItemCharges                     ;
    static int ItemInfo                        ;
    static int ItemInfoId                      ;

    static int ViewProjMatrices                ;
    static int ViewMatrix                      ;
    static int ProjectionMatrix                ;
    static int Renderer                        ;
    static int RendererWidth                   ;
    static int RendererHeight                  ;

    static int SpellSlotLevel                  ;
    static int SpellSlotTime                   ;
    static int SpellSlotCharges                ;
    static int SpellSlotTimeCharge             ;
    static int SpellSlotValue                  ;
    static int SpellSlotSpellInfo              ;
    static int SpellSlotDamage                 ;

    static int SpellInfoSpellData              ;
    static int SpellDataSpellName              ;
    static int SpellDataMissileName            ;

    static int ObjectManager                   ;
    static int LocalPlayer                     ;
    static int UnderMouseObject                ;

    static int MinimapObject                   ;
    static int MinimapObjectHud                ;
    static int MinimapHudPos                   ;
    static int MinimapHudSize                  ;

    static int ObjectMapCount                  ;
    static int ObjectMapRoot                   ;
    static int ObjectMapNodeNetId              ;
    static int ObjectMapNodeObject             ;

    static int ObjMissileName                  ;
    static int ObjMissileSpellCast             ;

    static int MissileSpellInfo                ;
    static int MissileSrcIdx                   ;
    static int MissileDestIdx                  ;
    static int MissileStartPos                 ;
    static int MissileEndPos                   ;

    static int SpellBookActiveSpellCast        ;
    static int SpellBookSpellSlots             ;
    static int SpellCastSpellInfo              ;
    static int SpellCastStartTime              ;
    static int SpellCastCastTime               ;
    static int SpellCastStart                  ;
    static int SpellCastEnd                    ;
    static int SpellCastSrcIdx                 ;
    static int SpellCastDestIdx                ;

    static int Chat                            ;
    static int ChatIsOpen                      ;

    static int FnCharacterDataStackUpdate      ;
    static int CharacterDataStack              ;
    static int CharacterDataStackSkinId        ;

    static int ObjBuffManager                  ;
    static int BuffStart                       ;
    static int BuffEnd                         ;
    static int BuffEntryBuff                   ;
    static int BuffEntryBuffStartTime          ;
    static int BuffEntryBuffEndTime            ;
    static int BuffName                        ;
    static int BuffCount1                      ;
    static int BuffCount2                      ;
};