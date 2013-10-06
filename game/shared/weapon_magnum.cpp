//====================== Created for Sleeples Mod by kaitek666 =======================//
//
// kaitek666:	Let's add Magnum pistol!
//
//=====================================================================================//

#include "cbase.h"
#include "weapon_sdkbase.h"

#if defined( CLIENT_DLL )

	#define CWeaponMAGNUM C_WeaponMAGNUM
	#include "c_sdk_player.h"

#else

	#include "sdk_player.h"

#endif


class CWeaponMAGNUM : public CWeaponSDKBase
{
public:
	DECLARE_CLASS( CWeaponMAGNUM, CWeaponSDKBase );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	DECLARE_ACTTABLE();
	
	CWeaponMAGNUM();

private:

	CWeaponMAGNUM( const CWeaponMAGNUM & );
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponMAGNUM, DT_WeaponMAGNUM )

BEGIN_NETWORK_TABLE( CWeaponMAGNUM, DT_WeaponMAGNUM )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponMAGNUM )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_magnum, CWeaponMAGNUM );
PRECACHE_WEAPON_REGISTER( weapon_magnum );



CWeaponMAGNUM::CWeaponMAGNUM()
{
}

acttable_t CWeaponMAGNUM::m_acttable[] = 
{
	{ ACT_MP_STAND_IDLE,					ACT_DOD_STAND_IDLE_TOMMY,				false },
	{ ACT_MP_CROUCH_IDLE,					ACT_DOD_CROUCH_IDLE_TOMMY,				false },
	//{ ACT_MP_PRONE_IDLE,					ACT_DOD_PRONE_AIM_TOMMY,				false },

	{ ACT_MP_RUN,							ACT_DOD_RUN_AIM_TOMMY,					false },
	{ ACT_MP_WALK,							ACT_DOD_WALK_AIM_TOMMY,					false },
	{ ACT_MP_CROUCHWALK,					ACT_DOD_CROUCHWALK_AIM_TOMMY,			false },
	//{ ACT_MP_PRONE_CRAWL,					ACT_DOD_PRONEWALK_IDLE_TOMMY,			false },
	//{ ACT_SPRINT,							ACT_DOD_SPRINT_IDLE_TOMMY,				false },

	{ ACT_MP_ATTACK_STAND_PRIMARYFIRE,		ACT_DOD_PRIMARYATTACK_TOMMY,			false },
	{ ACT_MP_ATTACK_CROUCH_PRIMARYFIRE,		ACT_DOD_PRIMARYATTACK_TOMMY,			false },
	//{ ACT_MP_ATTACK_PRONE_PRIMARYFIRE,		ACT_DOD_PRIMARYATTACK_PRONE_TOMMY,		false },
	{ ACT_MP_ATTACK_STAND_SECONDARYFIRE,	ACT_DOD_SECONDARYATTACK_TOMMY,			false },
	{ ACT_MP_ATTACK_CROUCH_SECONDARYFIRE,	ACT_DOD_SECONDARYATTACK_CROUCH_TOMMY,	false },
	//{ ACT_MP_ATTACK_PRONE_SECONDARYFIRE,	ACT_DOD_SECONDARYATTACK_PRONE_TOMMY,	false },

	{ ACT_MP_RELOAD_STAND,					ACT_DOD_RELOAD_TOMMY,					false },
	{ ACT_MP_RELOAD_CROUCH,					ACT_DOD_RELOAD_CROUCH_TOMMY,			false },
	//{ ACT_MP_RELOAD_PRONE,					ACT_DOD_RELOAD_PRONE_TOMMY,				false },

};

IMPLEMENT_ACTTABLE( CWeaponMAGNUM );

