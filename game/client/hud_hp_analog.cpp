	#include "cbase.h"
    #include "hud.h"
    #include "hud_hp_analog.h"
    #include "iclientmode.h"
    #include "hud_macros.h"
    #include "vgui_controls/controls.h"
    #include "vgui/ISurface.h"  
    #include "tier0/memdbgon.h"
    #include <vgui/ILocalize.h>
     
     
    CHud_hp_analog::CHud_hp_analog( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "Hud_hp_analog" )
    {
            vgui::Panel *pParent = g_pClientMode->GetViewport();
            SetParent( pParent );
     
            SetBgColor(Color(0,255,0,0));  // Kolor tla
            SetPaintBackgroundEnabled( false );  // Tlo ma byæ nie widoczne
     
            m_nTextureID = vgui::surface()->CreateNewTextureID();
            vgui::surface()->DrawSetTextureFile( m_nTextureID, "hud/pasehp" , true, false);
           
            SetHiddenBits(HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT);  // Kiedy element ma byæ NIE widoczny
    };
     
    DECLARE_HUDELEMENT( CHud_hp_analog );
     
    void CHud_hp_analog::Paint()
    {
            vgui::surface()->DrawSetColor(  255, 0, 0, 255 ); //RGBA
            vgui::surface()->DrawSetTexture( m_nTextureID );
            vgui::surface()->DrawTexturedRect(-200 + 2*m_iHealth, 0, 2*m_iHealth, 50 );
    }
     
    void CHud_hp_analog::OnThink()
    {
            int newHealth = 0;
            C_BasePlayer *local = C_BasePlayer::GetLocalPlayer();
            if ( local )
            {
                    // Never below zero
                    newHealth = max( local->GetHealth(), 0 );
            }
     
            // Only update the fade if we've changed health
            if ( newHealth == m_iHealth )
            {
                    return;
            }
     
            m_iHealth = newHealth;
     
            Paint();
    }
    

