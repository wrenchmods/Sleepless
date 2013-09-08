//========= Copyright © 2013, kaitek666, All rights reserved. ============//
//
// HEADER Wyswietla mozg w lewym dolnym rogu kolo zdrowia
//
//=============================================================================//

#include "hudelement.h"
#include <vgui_controls\Panel.h>

class CHUD_HEALTH_BG : public CHudElement, public vgui::Panel
{
 
   DECLARE_CLASS_SIMPLE( CHUD_HEALTH_BG, vgui::Panel );
 
   public:
     CHUD_HEALTH_BG( const char *pElementName );
 
   protected:
     virtual void Paint();
     int m_nTextureID;
 
  };