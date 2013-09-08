	

    #include "cbase.h"
    #include "hud.h"
    #include "c_basehlplayer.h"
    #include "hud_en_analog.h"
    #include "iclientmode.h"
    #include "hud_macros.h"
    #include "vgui_controls/controls.h"
    #include "vgui/ISurface.h"  
    #include "tier0/memdbgon.h"
    #include <vgui/ILocalize.h>
     
     
    ConVar max_last_change("cl_hud_mlc", "100", 0);
    ConVar anim_in_speed("cl_hud_ais", "4", 0);
    ConVar anim_out_speed("cl_hud_aos", "1", 0);
     
    CHud_en_analog::CHud_en_analog( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "Hud_en_analog" )
    {
            int i;
            char text1[64];
            char text2[64];
            vgui::Panel *pParent = g_pClientMode->GetViewport();
            SetParent( pParent );
     
            SetBgColor(Color(0,255,0,0));  // Kolor tla
            SetPaintBackgroundEnabled( false );  // Tlo ma byæ nie widoczne
     
            m_iAnim = 0;
            m_iLastChange = 0;
     
            for(i=0; i<17; ++i){
                    Q_snprintf(text1, sizeof(text1), "hud/paseen/paseen%02d", i);
                    m_nTextureID[i] = vgui::surface()->CreateNewTextureID();
                    vgui::surface()->DrawSetTextureFile( m_nTextureID[i], text1 , true, false);
                    Q_snprintf(text2, sizeof(text2), "ID: %d, Path: %s", i, text1);
    //              DevMsg(text2);
            }
           
            SetHiddenBits(HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT);  // Kiedy element ma byæ NIE widoczny
    };
     
    DECLARE_HUDELEMENT( CHud_en_analog );
     
    void CHud_en_analog::Paint()
    {
            int alpha = 255 - (m_iAnim * 16);
            char text[64];
            if(alpha == -1) ++alpha;
            Q_snprintf(text, sizeof(text), "m_iAnim: %d alpha: %d\n", m_iAnim, alpha);
    //      DevMsg(text);
            vgui::surface()->DrawSetTexture( m_nTextureID[m_iAnim] );
            vgui::surface()->DrawSetColor(  255, 255, 255, alpha ); //RGBA
            vgui::surface()->DrawTexturedRect(-200 + 2*m_flSuitPower, m_iAnim, 2*m_flSuitPower, m_iAnim + 50 );
    }
     
     
    void CHud_en_analog::OnThink()
    {
            int newPower = 0;
            char text[64];
            C_BaseHLPlayer *local = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
            if ( local )
            {
                    // Never below zero
                    newPower = max( local->m_HL2Local.m_flSuitPower, 0 );
            }
    //      DevMsg("Think!\n");
            // Only update the fade if we've changed health
            if ( newPower == m_flSuitPower )
            {
    //              DevMsg("No change\n");
                    ++m_iLastChange;
                    if(m_iLastChange >= max_last_change.GetInt()){
                            if( m_iAnim < 16 ){
                                    Q_snprintf(text, sizeof(text), "Anim: %d < 16 \n ++ing \n", m_iAnim);
    //                              DevMsg(text);
                                    m_iAnim+=anim_out_speed.GetInt();
                            } else {
                                    Q_snprintf(text, sizeof(text), "Anim: %d >= 16 \n return \n", m_iAnim);
    //                              DevMsg(text);
                                    return;
                            }
                    }
            } else {
    //              DevMsg("Change!\n");
                    m_iLastChange = 0;
                    if( m_iAnim > 0){
                            Q_snprintf(text, sizeof(text), "Anim: %d > 0 \n --ing \n", m_iAnim);
    //                      DevMsg(text);
                            m_iAnim-=anim_in_speed.GetInt();
                    }
            }
           
            m_flSuitPower = newPower;
     
            Paint();
    }
     
    bool CHud_en_analog::ShouldDraw()
    {
            bool bNeedsDraw = false;
     
    //      DevMsg("ShouldDraw?\n");
            C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
            if ( !pPlayer )
                    return false;
     
            // needs draw if suit power changed or animation in progress
            bNeedsDraw = ( ( pPlayer->m_HL2Local.m_flSuitPower != m_flSuitPower ) || (m_iAnim < 16));
           
    //      DevMsg( bNeedsDraw ? "Yes\n" : "No\n" );
     
            return ( bNeedsDraw && CHudElement::ShouldDraw() );
    }

