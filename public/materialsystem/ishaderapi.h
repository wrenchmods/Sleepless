//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: NOTE: This file is for backward compat!
// We'll get rid of it soon. Most of the contents of this file were moved
// into shaderpi/ishadershadow.h, shaderapi/ishaderdynamic.h, or
// shaderapi/shareddefs.h
//
// $NoKeywords: $
//
//===========================================================================//

#ifndef ISHADERAPI_MS_H
#define ISHADERAPI_MS_H

#ifdef _WIN32
#pragma once
#endif

#include <shaderapi/shareddefs.h>
#include <shaderapi/ishadershadow.h>
#include <shaderapi/ishaderdynamic.h>


//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------
class IMaterialVar;


//-----------------------------------------------------------------------------
// Methods that can be called from the SHADER_INIT blocks of shaders
//-----------------------------------------------------------------------------
abstract_class IShaderInit
{
public:
	// Loads up a texture
	virtual void LoadTexture( IMaterialVar *pTextureVar, const char *pTextureGroupName ) = 0; 
	virtual void LoadBumpMap( IMaterialVar *pTextureVar, const char *pTextureGroupName ) = 0;
	virtual void LoadCubeMap( IMaterialVar **ppParams, IMaterialVar *pTextureVar ) = 0;
};

//-----------------------------------------------------------------------------
// Stencil state
//-----------------------------------------------------------------------------
struct ShaderStencilState_t
{
	bool m_bEnable;
	ShaderStencilOp_t m_FailOp;
	ShaderStencilOp_t m_ZFailOp;
	ShaderStencilOp_t m_PassOp;
	ShaderStencilFunc_t m_CompareFunc;
	int m_nReferenceValue;
	uint32 m_nTestMask;
	uint32 m_nWriteMask;

#if defined( _X360 )
	bool m_bHiStencilEnable;
	bool m_bHiStencilWriteEnable;
	ShaderHiStencilFunc_t m_HiStencilCompareFunc;
	int m_nHiStencilReferenceValue;
#endif

	ShaderStencilState_t()
	{
		m_bEnable = false;
		m_PassOp = m_FailOp = m_ZFailOp = SHADER_STENCILOP_KEEP;
		m_CompareFunc = SHADER_STENCILFUNC_ALWAYS;
		m_nReferenceValue = 0;
		m_nTestMask = m_nWriteMask = 0xFFFFFFFF;

#if defined( _X360 )
		m_bHiStencilEnable = false;
		m_bHiStencilWriteEnable = false;
		m_HiStencilCompareFunc = SHADER_HI_STENCILFUNC_EQUAL;
		m_nHiStencilReferenceValue = 0;
#endif
	}
};


#endif // ISHADERAPI_MS_H
