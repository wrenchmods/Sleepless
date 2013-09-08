#include "hud.h"
#include "cbase.h"
#include "hud_import.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"

#include "tier0/memdbgon.h"

static ConVar show_beta("show_beta", "0", 0, "toggles beta icon in upper right corner");

using namespace vgui;

DECLARE_HUDELEMENT( CHudImport );

CHudImport::CHudImport( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudImport" )
{
    Panel *pParent = g_pClientMode->GetViewport();
    SetParent( pParent );  
  
    SetVisible( false );
    SetAlpha( 25 ); //AW Create Texture for Looking around
    m_nImport = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_nImport, "HUD/import" , true, true);

   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
}

void CHudImport::Paint()
{
    SetPaintBorderEnabled(false);
    surface()->DrawSetTexture( m_nImport );
    surface()->DrawTexturedRect( 2, 2, 128, 128 );
}

void CHudImport::togglePrint()
{
    if (!show_beta.GetBool())
        this->SetVisible(false);
    else
        this->SetVisible(true);
}

void CHudImport::OnThink()
{
    togglePrint();

    BaseClass::OnThink();
}