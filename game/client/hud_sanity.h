//========= Copyright © 2013, kaitek666, All rights reserved. ============//
//
// HEADER Wyswietla mozg w lewym dolnym rogu kolo zdrowia
//
//=============================================================================//

#include "hudelement.h"
#include <vgui_controls\Panel.h>

class Chud_sanity : public CHudElement, public vgui::Panel
{
 
   DECLARE_CLASS_SIMPLE( Chud_sanity, vgui::Panel );
 
   public:
     Chud_sanity( const char *pElementName );
 
   protected:
     virtual void Paint();
     int m_nTextureID;
 
  };