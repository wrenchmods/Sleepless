
#include "cbase.h"
#include "weapon_sdkmelee.h"

#if defined( CLIENT_DLL )

	#include "c_sdk_player.h"

#else

	#include "sdk_player.h"
    #include "te_effect_dispatch.h"

#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define BLUDGEON_HULL_DIM                16
static const Vector g_bludgeonMins(-BLUDGEON_HULL_DIM,-BLUDGEON_HULL_DIM,-BLUDGEON_HULL_DIM);
static const Vector g_bludgeonMaxs(BLUDGEON_HULL_DIM,BLUDGEON_HULL_DIM,BLUDGEON_HULL_DIM);

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponSDKMelee, DT_WeaponSDKMelee )

BEGIN_NETWORK_TABLE( CWeaponSDKMelee, DT_WeaponSDKMelee )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponSDKMelee )
END_PREDICTION_DATA()

CWeaponSDKMelee::CWeaponSDKMelee() {
	m_bFiresUnderwater = true;
}

void CWeaponSDKMelee::Swing(bool bIsSecondary) {
//    DevMsg("Swing\n");
	trace_t traceHit;

	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if (!pOwner) {
//		DevMsg("No owner\n");
		return;
	}

	Vector swingStart = pOwner->Weapon_ShootPosition();
	Vector forward;
	pOwner->EyeVectors(&forward, NULL, NULL);

	Vector swingEnd = swingStart + forward * GetRange();
	UTIL_TraceLine(swingStart, swingEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &traceHit);
	Activity nHitActivity = ACT_VM_PRIMARYATTACK;

#ifndef CLIENT_DLL
	CTakeDamageInfo triggerInfo(GetOwner(), GetOwner(), GetDamageForActivity(nHitActivity), DMG_CLUB);
	TraceAttackToTriggers(triggerInfo, traceHit.startpos, traceHit.endpos, vec3_origin);
#endif

	if (traceHit.fraction == 1.0) {
		float bludgeonHullRadius = 1.732f * BLUDGEON_HULL_DIM;
        // Back off by hull "radius"
        swingEnd -= forward * bludgeonHullRadius;

        UTIL_TraceHull( swingStart, swingEnd, g_bludgeonMins, g_bludgeonMaxs, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &traceHit );
        if ( traceHit.fraction < 1.0 && traceHit.m_pEnt ) {
            Vector vecToTarget = traceHit.m_pEnt->GetAbsOrigin() - swingStart;
            VectorNormalize( vecToTarget );
				
			float dot = vecToTarget.Dot( forward );

            // YWB: Make sure they are sort of facing the guy at least...
            if ( dot < 0.70721f ) {
                // Force amiss
                traceHit.fraction = 1.0f;
            } else {
                nHitActivity = ChooseIntersectionPointAndActivity( traceHit, g_bludgeonMins, g_bludgeonMaxs, pOwner );
            }
        }
    }

	WeaponSound( SINGLE );
 //   DevMsg("Sound\n");

	// --- MISS ---
    if ( traceHit.fraction == 1.0f ) {
		nHitActivity = bIsSecondary ? /*ACT_VM_MISSCENTER2*/ACT_VM_PRIMARYATTACK : /*ACT_VM_MISSCENTER*/ACT_VM_PRIMARYATTACK;
		Vector testEnd = swingStart + forward * GetRange();
		ImpactWater(swingStart, testEnd);
	} else {
		Hit( traceHit, nHitActivity );
	}

//    DevMsg("Anim\n");
	SendWeaponAnim( nHitActivity );
	pOwner->SetAnimation( PLAYER_ATTACK1 );

	//Setup our next attack times
    m_flNextPrimaryAttack = gpGlobals->curtime + GetFireRate();
    m_flNextSecondaryAttack = gpGlobals->curtime + SequenceDuration();
}

bool CWeaponSDKMelee::ImpactWater( const Vector &start, const Vector &end )
{
//        DevMsg("Water\n");

        //FIXME: This doesn't handle the case of trying to splash while being underwater, but that's not going to look good
        //                 right now anyway...
        
        // We must start outside the water
        if ( UTIL_PointContents( start, CONTENTS_WATER|CONTENTS_SLIME ) & (CONTENTS_WATER|CONTENTS_SLIME))
                return false;

        // We must end inside of water
        if ( !(UTIL_PointContents( end, CONTENTS_WATER|CONTENTS_SLIME ) & (CONTENTS_WATER|CONTENTS_SLIME)))
                return false;

        trace_t        waterTrace;

        UTIL_TraceLine( start, end, (CONTENTS_WATER|CONTENTS_SLIME), GetOwner(), COLLISION_GROUP_NONE, &waterTrace );

        if ( waterTrace.fraction < 1.0f )
        {
#ifndef CLIENT_DLL
                CEffectData        data;

                data.m_fFlags = 0;
                data.m_vOrigin = waterTrace.endpos;
                data.m_vNormal = waterTrace.plane.normal;
                data.m_flScale = 8.0f;

                // See if we hit slime
                if ( waterTrace.contents & CONTENTS_SLIME )
                {
                        data.m_fFlags |= FX_WATER_IN_SLIME;
                }

                DispatchEffect( "watersplash\n", data );                        
#endif
        }

        return true;
}

void CWeaponSDKMelee::Hit( trace_t &traceHit, Activity nHitActivity ) {
//	        DevMsg("Hit!\n");
	        CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
        
        //Do view kick
//        AddViewKick();

        CBaseEntity        *pHitEntity = traceHit.m_pEnt;

        //Apply damage to a hit target
        if ( pHitEntity != NULL )
        {
                Vector hitDirection;
                pPlayer->EyeVectors( &hitDirection, NULL, NULL );
                VectorNormalize( hitDirection );

#ifndef CLIENT_DLL
                CTakeDamageInfo info( GetOwner(), GetOwner(), GetDamageForActivity( nHitActivity ), DMG_CLUB );

                if( pPlayer && pHitEntity->IsNPC() )
                {
                        // If bonking an NPC, adjust damage.
                        info.AdjustPlayerDamageInflictedForSkillLevel();
                }

                CalculateMeleeDamageForce( &info, hitDirection, traceHit.endpos );

                pHitEntity->DispatchTraceAttack( info, hitDirection, &traceHit );
                ApplyMultiDamage();

                // Now hit all triggers along the ray that...
                TraceAttackToTriggers( info, traceHit.startpos, traceHit.endpos, hitDirection );
#endif
                WeaponSound( MELEE_HIT );
        }

        // Apply an impact effect
        ImpactEffect( traceHit );
}

Activity CWeaponSDKMelee::ChooseIntersectionPointAndActivity( trace_t &hitTrace, const Vector &mins, const Vector &maxs, CBasePlayer *pOwner )
{
        int                        i, j, k;
        float                distance;
        const float        *minmaxs[2] = {mins.Base(), maxs.Base()};
        trace_t                tmpTrace;
        Vector                vecHullEnd = hitTrace.endpos;
        Vector                vecEnd;

        distance = 1e6f;
        Vector vecSrc = hitTrace.startpos;

        vecHullEnd = vecSrc + ((vecHullEnd - vecSrc)*2);
        UTIL_TraceLine( vecSrc, vecHullEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &tmpTrace );
        if ( tmpTrace.fraction == 1.0 )
        {
                for ( i = 0; i < 2; i++ )
                {
                        for ( j = 0; j < 2; j++ )
                        {
                                for ( k = 0; k < 2; k++ )
                                {
                                        vecEnd.x = vecHullEnd.x + minmaxs[i][0];
                                        vecEnd.y = vecHullEnd.y + minmaxs[j][1];
                                        vecEnd.z = vecHullEnd.z + minmaxs[k][2];

                                        UTIL_TraceLine( vecSrc, vecEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &tmpTrace );
                                        if ( tmpTrace.fraction < 1.0 )
                                        {
                                                float thisDistance = (tmpTrace.endpos - vecSrc).Length();
                                                if ( thisDistance < distance )
                                                {
                                                        hitTrace = tmpTrace;
                                                        distance = thisDistance;
                                                }
                                        }
                                }
                        }
                }
        }
        else
        {
                hitTrace = tmpTrace;
        }


        return ACT_VM_PRIMARYATTACK;
}

void CWeaponSDKMelee::PrimaryAttack() {
//	DevMsg("Primary attack\n");
	Swing(false);
}

void CWeaponSDKMelee::SecondaryAttack() {
	Swing(true);
}

void CWeaponSDKMelee::ImpactEffect( trace_t &traceHit )
{
        // See if we hit water (we don't do the other impact effects in this case)
        if ( ImpactWater( traceHit.startpos, traceHit.endpos ) )
                return;

        //FIXME: need new decals
        UTIL_ImpactTrace( &traceHit, DMG_CLUB );
}