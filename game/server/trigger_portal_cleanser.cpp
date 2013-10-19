//========================== Created by Pawel C. =============================//
//
// Purpose: Replica of trigger_portal_cleanser.
// It removes objects when the player touches it. 
// It is used to create the "Material Emancipation Grid" / "Fizzler" particle-field in Portal. 
//
//=============================================================================//

// TO DO: (PL)
// -Dla metalowych npc nale¿y dodaæ: bool GetCleanserInfluence() { return true } //(reszta npc false).
// -Siatka nie powinna odzia³ywaæ z ¿ywymi npc.
// -Nale¿y usun¹æ ten "dziwny" sposób polegaj¹cy na tworzeniu nowego obiektu - zastêpczego (wziête z portal 1).
// -Niektórzy (lub nawet wszyscy podlegaj¹cy dematerializacji) npc powinni posiadaæ
// w³asn¹ wersjê funkcji dematerializuj¹cej np rakieta, granat lub wie¿yczka (ta z portal'a).

#include "cbase.h"
#include "triggers.h"
#include "player_pickup.h"
#include "model_types.h"
#include "ai_basenpc.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


ConVar portal_cleanser_debug( "portal_cleanser_debug", "1", FCVAR_CHEAT, "Turn on debugging for trigger_portal_cleanser." );

class CTriggerPortalCleanser : public CBaseTrigger
{
public:
	DECLARE_DATADESC();
	DECLARE_CLASS( CTriggerPortalCleanser, CBaseTrigger );

	virtual void Spawn( void );
	virtual void StartTouch( CBaseEntity *pOther );
	virtual void EndTouch(CBaseEntity *pOther);

private:
	bool CTriggerPortalCleanser::TransferPhysicsObject( CBaseEntity *pFrom, CBaseEntity *pTo, bool wakeUp );
	CBaseEntity *CTriggerPortalCleanser::CreateSimplePhysicsObject( CBaseEntity *pEntity );

	COutputEvent	m_OnDissolve;
	COutputEvent	m_OnDissolveBox;
	//COutputEvent	m_OnFizzle
};

LINK_ENTITY_TO_CLASS( trigger_portal_cleanser, CTriggerPortalCleanser );

BEGIN_DATADESC( CTriggerPortalCleanser )
	DEFINE_OUTPUT( m_OnDissolve, "OnDissolve" ),
	DEFINE_OUTPUT( m_OnDissolveBox, "OnDissolveBox" )
	//DEFINE_OUTPUT( m_OnFizzle, "OnFizzle" )
END_DATADESC()


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTriggerPortalCleanser::Spawn( void )
{
	BaseClass::Spawn();

	InitTrigger();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pOther - 
//-----------------------------------------------------------------------------
void CTriggerPortalCleanser::EndTouch(CBaseEntity *pOther)
{
	BaseClass::EndTouch(pOther);

	if ( portal_cleanser_debug.GetBool() )
	{
		Msg("%s END-TOUCH: for %s\n", GetDebugName(), pOther->GetDebugName() );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Called when an entity starts touching us.
// Input  : pOther - The entity that is touching us.
//-----------------------------------------------------------------------------
void CTriggerPortalCleanser::StartTouch( CBaseEntity *pOther )
{
	if ( PassesTriggerFilters(pOther) )
	{
		EHANDLE hOther;
		hOther = pOther;
		
		bool bAdded = false;
		if ( m_hTouchingEntities.Find( hOther ) == m_hTouchingEntities.InvalidIndex() )
		{
			m_hTouchingEntities.AddToTail( hOther );
			bAdded = true;
		}

		m_OnStartTouch.FireOutput(pOther, this);

		if ( bAdded && ( m_hTouchingEntities.Count() == 1 ) )
		{
			// First entity to touch us that passes our filters
			m_OnStartTouchAll.FireOutput( pOther, this );
		}

		if ( portal_cleanser_debug.GetBool() )
		{
			Msg("%s START-TOUCH: for %s\n", GetDebugName(), pOther->GetDebugName() );
		}

		if ( !pOther->IsPlayer() )
		{
			CBaseAnimating *pAnim = pOther->GetBaseAnimating();
			if ( !pAnim )
				return;

			// Can't dissolve twice.
			if ( pAnim->IsDissolving() )
				return;

			// Force player to drop this object.
			Pickup_ForcePlayerToDropThisObject( pOther );

			if ( !FClassnameIs( pOther, "prop_physics" ) )
			{
				if ( FClassnameIs( pOther, "simple_physics_prop" ) || FClassnameIs( pOther, "simple_physics_brush" ) )
				{
					// simple_physics_prop ?
					return;
				}

				if ( portal_cleanser_debug.GetBool() )
				{
					Msg("%s IS CREATING: simple_physics_prop\n", GetDebugName());
				}
				// Other object needs to be replaced by simple_physics_prop.
				pOther = CreateSimplePhysicsObject( pOther );

				// Dissolve the entity.
				CBaseAnimating *pAnim = pOther->GetBaseAnimating();
				pAnim->Dissolve( NULL, gpGlobals->curtime, false, ENTITY_DISSOLVE_NORMAL );

				if ( portal_cleanser_debug.GetBool() )
				{
					Msg("%s DISSOLVE SIMPLE PHYSICS: %s\n", GetDebugName(), pOther->GetDebugName() );
				}

				// Outputs
				m_hActivator = pOther;
				m_OnDissolve.FireOutput(m_hActivator, this);

				return;
			}

			IPhysicsObject *pPhysics = pOther->VPhysicsGetObject();
			if ( pPhysics )
			{
				// Dissolve the entity.
				pAnim->Dissolve( NULL, gpGlobals->curtime, false, ENTITY_DISSOLVE_NORMAL );

				if ( portal_cleanser_debug.GetBool() )
				{
					Msg("%s DISSOLVE PHYSICS: %s\n", GetDebugName(), pOther->GetDebugName() );
				}

				// Turn off the gravity for this object.
				pPhysics->EnableGravity( false );

				// Slow down and push up the object.
				Vector vecVelocity, vecAngular;
				pPhysics->GetVelocity( &vecVelocity, &vecAngular );
				vecVelocity /= 2;
				vecAngular /= 2;
				vecVelocity.z += 10;
				pPhysics->SetVelocity( &vecVelocity, &vecAngular );

				// Outputs
				m_hActivator = pOther;
				m_OnDissolve.FireOutput(m_hActivator, this);

				const char *pModelName = STRING( pOther->GetModelName() );
				if ( Q_strstr( pModelName, "models/props/metal_box.mdl" ) )
				{
					m_OnDissolveBox.FireOutput(m_hActivator, this);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CTriggerPortalCleanser::TransferPhysicsObject( CBaseEntity *pFrom, CBaseEntity *pTo, bool wakeUp )
{
	IPhysicsObject *pVPhysics = pFrom->VPhysicsGetObject();
	if ( !pVPhysics || pVPhysics->IsStatic() )
		return false;

	Vector vecVelocity, vecAngular;
	pVPhysics->GetVelocity( &vecVelocity, &vecAngular );

	// clear out the pointer so it won't get deleted
	pFrom->VPhysicsSwapObject( NULL );
	// remove any AI behavior bound to it
	pVPhysics->RemoveShadowController();
	// transfer to the new owner
	pTo->VPhysicsSetObject( pVPhysics );
	pVPhysics->SetGameData( (void *)pTo );
	pTo->VPhysicsUpdate( pVPhysics );
	
	// may have been temporarily disabled by the old object
	pVPhysics->EnableMotion( true );
	pVPhysics->EnableGravity( false );
	
	// Slow down and push up the object.
	vecVelocity /= 2;
	vecAngular /= 2;
	vecVelocity.z += 10;
	pVPhysics->SetVelocity( &vecVelocity, &vecAngular );

	// Update for the new entity solid type
	pVPhysics->RecheckCollisionFilter();
	if ( wakeUp )
	{
		pVPhysics->Wake();
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CBaseEntity *CTriggerPortalCleanser::CreateSimplePhysicsObject( CBaseEntity *pEntity )
{
	CBaseEntity *pPhysEntity = NULL;
	int modelindex = pEntity->GetModelIndex();
	const model_t *model = modelinfo->GetModel( modelindex );
	if ( model && modelinfo->GetModelType(model) == mod_brush )
	{
		pPhysEntity = CreateEntityByName( "simple_physics_brush" );
	}
	else
	{
		pPhysEntity = CreateEntityByName( "simple_physics_prop" );
	}

	pPhysEntity->KeyValue( "model", STRING(pEntity->GetModelName()) );
	pPhysEntity->SetAbsOrigin( pEntity->GetAbsOrigin() );
	pPhysEntity->SetAbsAngles( pEntity->GetAbsAngles() );
	pPhysEntity->Spawn();
	if ( !TransferPhysicsObject( pEntity, pPhysEntity, true ) )
	{
		pPhysEntity->VPhysicsInitNormal( SOLID_VPHYSICS, 0, false );
	}

	pEntity->Remove();

	return pPhysEntity;
}
