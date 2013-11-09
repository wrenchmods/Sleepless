#include "cbase.h"
#include "hud.h"
#include "hud_macros.h"
#include "hudelement.h" 

class CHudStatsHunger : public CHudElement, public vgui::Panel 
{ 
  
   DECLARE_CLASS_SIMPLE( CHudStatsHunger, vgui::Panel ); 
  
   public: 
     CHudStatsHunger(const char *pElementName ); 
  
   protected: 
     virtual void Paint(); 
     int m_nTextureID; 
  
  };