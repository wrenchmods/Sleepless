//========= Copyright © 2013, kaitek666, All rights reserved. ============//
//
// CPP Wyswietla mozg w lewym dolnym rogu kolo zdrowia
//
//=============================================================================//

#include "hud.h"
#include "cbase.h"
#include "hud_sanity.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h" 
#include "tier0/memdbgon.h"

Chud_sanity::Chud_sanity( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "hud_sanity" )
{
   vgui::Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );

   SetBgColor(Color(0,255,0,0));  // Kolor tla
   SetPaintBackgroundEnabled( false );  // Tlo ma byæ nie widoczne

	m_nTextureID = vgui::surface()->CreateNewTextureID();
	vgui::surface()->DrawSetTextureFile( m_nTextureID, "hud/test" , true, false); 
   
   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT | HIDEHUD_FLASHLIGHT );  // Kiedy element ma byæ NIE widoczny
};

DECLARE_HUDELEMENT( Chud_sanity );

void Chud_sanity::Paint()
{
vgui::surface()->DrawSetColor(  255, 0, 0, 255 ); //RGBA
vgui::surface()->DrawSetTexture( m_nTextureID );
vgui::surface()->DrawTexturedRect( 0, 0, 512, 128 ); 
}