	    #include "hudelement.h"
    #include <vgui_controls/Panel.h>
     
    class CHud_hp_analog : public CHudElement, public vgui::Panel
    {
     
       DECLARE_CLASS_SIMPLE( CHud_hp_analog, vgui::Panel );
     
       public:
         CHud_hp_analog( const char *pElementName );
             void OnThink();
     
       protected:
         virtual void Paint();
         int m_nTextureID;
             int m_iHealth;
     
      };

