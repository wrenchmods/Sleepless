//========= Copyright © 2013, kaitek666, All rights reserved. ============//
//
// CPP Wyswietla mozg w lewym dolnym rogu kolo zdrowia
//
//=============================================================================//

#include "hud.h"
#include "cbase.h"
#include "HUD_HEALTH_BG.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h" 
#include "tier0/memdbgon.h"

CHUD_HEALTH_BG::CHUD_HEALTH_BG( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "hud_sanity" )
{
   vgui::Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );

   SetBgColor(Color(0,255,0,0));  // Kolor tla
   SetPaintBackgroundEnabled( false );  // Tlo ma byæ nie widoczne

   
   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT | HIDEHUD_FLASHLIGHT );  // Kiedy element ma byæ NIE widoczny
};

DECLARE_HUDELEMENT( CHUD_HEALTH_BG );

void CHUD_HEALTH_BG::Paint()
{
vgui::surface()->DrawSetColor(  131, 139, 139, 140 ); //RGBA
vgui::surface()->DrawFilledRect( 0, 480-20, 20, 480 ); //x0,y0,x1,y1 

}