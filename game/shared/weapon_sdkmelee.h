
#include "weapon_sdkbase.h"

#if defined( CLIENT_DLL )
#define CWeaponSDKMelee C_WeaponSDKMelee
#endif

class CWeaponSDKMelee : public CWeaponSDKBase {
	DECLARE_CLASS(CWeaponSDKMelee, CWeaponSDKBase);
public:
	CWeaponSDKMelee();
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();
	virtual void PrimaryAttack( void );
	virtual void SecondaryAttack( void );
	virtual float GetRange( void ) { return 100.0f; } // Just for now
	virtual float GetDamageForActivity(Activity hitActivity) {return 100.0f;} // Just for now
private:
	void Swing(bool bIsSecondary);
	void Hit( trace_t &traceHit, Activity nHitActivity );
	bool ImpactWater( const Vector &start, const Vector &end );
	Activity ChooseIntersectionPointAndActivity( trace_t &hitTrace, const Vector &mins, const Vector &maxs, CBasePlayer *pOwner );
protected:
    virtual void ImpactEffect( trace_t &trace );

};