//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef FLASHLIGHTEFFECT_H
#define FLASHLIGHTEFFECT_H
#ifdef _WIN32
#pragma once
#endif
#define FLASHLIGHT_DELAY

struct dlight_t;


class CFlashlightEffect
{
public:

	CFlashlightEffect(int nEntIndex = 0);
	// @Deferred - Biohazard
	virtual ~CFlashlightEffect();

	virtual void UpdateLight(const Vector &vecPos, const Vector &vecDir, const Vector &vecRight, const Vector &vecUp, int nDistance);
	bool m_bFirstUpdate;
	void TurnOn();
	void TurnOff();
	bool IsOn( void ) { return m_bIsOn;	}

	ClientShadowHandle_t GetFlashlightHandle( void ) { return m_FlashlightHandle; }
	void SetFlashlightHandle( ClientShadowHandle_t Handle ) { m_FlashlightHandle = Handle;	}
	
protected:

	// @Deferred - Biohazard
	virtual void UpdateLightProjection( FlashlightState_t &state );
	void UpdateLightTopDown(const Vector &vecPos, const Vector &vecDir, const Vector &vecRight, const Vector &vecUp);

	void LightOff();
	void LightOffOld();
	void LightOffNew();

	float m_flCurrentPullBackDist;
	float m_FarZAtten;

	void UpdateLightNew(const Vector &vecPos, const Vector &vecDir, const Vector &vecRight, const Vector &vecUp);
	void UpdateLightOld(const Vector &vecPos, const Vector &vecDir, int nDistance);

	bool m_bIsOn;
	int m_nEntIndex;
	ClientShadowHandle_t m_FlashlightHandle;

	// Vehicle headlight dynamic light pointer
	dlight_t *m_pPointLight;
	float m_flDistMod;

	// Texture for flashlight
	CTextureReference m_FlashlightTexture;

	#ifdef FLASHLIGHT_DELAY
	Vector lightPos;
	Vector lightDir;
	Vector lightUp;
	Vector lightRight;
	#endif

};

class CHeadlightEffect : public CFlashlightEffect
{
public:

	// @Deferred - Biohazard
	void TurnOnFlashlight( int nEntIndex = 0, const char *pszTextureName = NULL, float flFov = 0.0f, float flFarZ = 0.0f, float flLinearAtten = 0.0f );
	
	CHeadlightEffect();
	~CHeadlightEffect();

	virtual void UpdateLight(const Vector &vecPos, const Vector &vecDir, const Vector &vecRight, const Vector &vecUp, int nDistance);
};



#endif // FLASHLIGHTEFFECT_H
