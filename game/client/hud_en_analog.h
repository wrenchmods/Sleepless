	

    #include "hudelement.h"
    #include <vgui_controls/Panel.h>
     
    #pragma once
     
    class CHud_en_analog : public CHudElement, public vgui::Panel
    {
     
       DECLARE_CLASS_SIMPLE( CHud_en_analog, vgui::Panel );
     
       public:
        CHud_en_analog( const char *pElementName );
            void OnThink();
            bool ShouldDraw( void );
     
       protected:
         virtual void Paint();
         int m_nTextureID[17];
             int m_flSuitPower;
             int m_iAnim;
             int m_iLastChange;
     
             friend class CHud_ml_analog;   // Hud_ml_analog jest nad Hud_en_analog wiêc musi siê razem z nim podnosiæ.
             friend class CHud_tlo;                 // Hud_tlo musi sie rozci¹gaæ, gdy Hud_en_analog siê podnosi
     
      };

