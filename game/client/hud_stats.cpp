//============ Copyright © 2013, kaitek666, All rights reserved. ==============//
//
// kaitek666:	Stats for hunger
//
//=============================================================================//

#include "cbase.h"
#include "weapon_selection.h"
#include "iclientmode.h"
#include "clientmode_sdk.h"
//#include "clientmode_sdk.cpp" // Geniusz.....
#include "history_resource.h"
#include "input.h"

#include <KeyValues.h>
#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui_controls/AnimationController.h>
#include <vgui_controls/Panel.h>

#include "vgui/ILocalize.h"

// memdbgon, as fuckin' always, must be the last include file in .cpp file!!!
#include "tier0/memdbgon.h"

class CHudStats : public CHudElement, public vgui::Panel 
{ 
  
   DECLARE_CLASS_SIMPLE( CHudStats, vgui::Panel ); 
  
   public: 
     CHudStats(const char *pElementName ); 
  
   protected: 
     virtual void Paint(); 
     int m_nTextureID; 
  
};

CHudStats::CHudStats( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudStats" ) 
{ 
   vgui::Panel *pParent = GetClientMode()->GetViewport(); 
   SetParent( pParent ); 

   SetBgColor(Color(0,255,0,0));  // bg color
   SetPaintBackgroundEnabled( false );  // invisible bg
    
   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT | HIDEHUD_FLASHLIGHT );  // not visible
}; 

DECLARE_HUDELEMENT( CHudStats ); 

void CHudStats::Paint() 
{ 

// get the right font handle for this scheme 
vgui::IScheme *pScheme = vgui::scheme()->GetIScheme(GetScheme()); 

// title
vgui::HFont hFont = pScheme->GetFont( "DefaultSleeplessSmall" ); 
vgui::surface()->DrawSetTextFont( hFont ); // set the font 
vgui::surface()->DrawSetTextColor( 0, 30, 255, 255 ); // full red 
vgui::surface()->DrawSetTextPos( 10, 10 ); // x,y position 
vgui::surface()->DrawPrintText( L"BILL STEUER", 11 ); // print text 

// background
vgui::surface()->DrawSetColor(  255, 0, 0, 150 ); //RGBA 
vgui::surface()->DrawFilledRect( 0, 0, 300, 200 ); //x0,y0,x1,y1 
}