//========= Copyright © 2006, Valde Productions, All rights reserved. ============//
//
// Purpose: Display Main Menu images, handles rollovers as well
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "vgui_Panel_MainMenu.h"
#include "vgui_controls/Frame.h"
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vgui/IInput.h>

#include "vgui_controls/Button.h"
#include "vgui_controls/ImagePanel.h"

using namespace vgui;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: Displays the logo panel
//-----------------------------------------------------------------------------
class CMainMenu : public vgui::Frame
{
   DECLARE_CLASS_SIMPLE(CMainMenu, vgui::Frame);

public:
   CMainMenu( vgui::VPANEL parent );
   ~CMainMenu();

   virtual void OnCommand(const char *command);

   virtual void ApplySchemeSettings( vgui::IScheme *pScheme )
   {
      
      BaseClass::ApplySchemeSettings( pScheme );
   }

   // The panel background image should be square, not rounded.
   virtual void PaintBackground()
   {
      SetBgColor(Color(0,0,0,0));
      SetPaintBackgroundType( 0 );
      BaseClass::PaintBackground();
   }
   virtual void PerformLayout()
   {
      // re-position
      SetPos(vgui::scheme()->GetProportionalScaledValue(defaultX), vgui::scheme()->GetProportionalScaledValue(defaultY));

      BaseClass::PerformLayout();
   }
   void CMainMenu::PerformDefaultLayout()
   {
      m_pButtonStartgame->SetPos(0, 0);
      m_pImgStartgame->SetPos(0,0);
      m_pButtonLoadgame->SetPos(0, 64);
      m_pImgLoadgame->SetPos(0,64);
      m_pButtonOptions->SetPos(0, 128);
      m_pImgOptions->SetPos(0,128);
      m_pButtonQuit->SetPos(0, 184);
      m_pImgQuit->SetPos(0,184);

      m_pImgSavegame->SetVisible(false);
      m_pButtonSavegame->SetVisible(false);

      m_pImgResumegame->SetVisible(false);
      m_pButtonResumegame->SetVisible(false);

      InRolloverResumegame=false;
      InRolloverStartgame=false;
      InRolloverLoadgame=false;
      InRolloverOptions=false;
      InRolloverQuit=false;
   }

   virtual void OnThink()
   {
      // In-game, everything will be in different places than at the root menu!
      if (InGame() && !InGameLayout) {
         DevMsg("Performing menu layout\n");
         int dy = 128; // delta y, shift value
         int x,y;
         // Resume
         m_pButtonResumegame->SetPos(0,0);
         m_pImgResumegame->SetPos(0,0);
         m_pButtonResumegame->SetVisible(true);
         m_pImgResumegame->SetVisible(true);

         m_pButtonStartgame->GetPos(x,y);
         m_pButtonStartgame->SetPos(x,y+dy);
         m_pImgStartgame->GetPos(x,y);
         m_pImgStartgame->SetPos(x,y+dy);

         m_pButtonLoadgame->GetPos(x,y);
         m_pButtonLoadgame->SetPos(x,y+dy);
         m_pImgLoadgame->GetPos(x,y);
         m_pImgLoadgame->SetPos(x,y+dy);

         // Save game
         m_pButtonSavegame->SetPos(x,y+(2*dy));
         m_pImgSavegame->SetPos(x,y+(2*dy));
         m_pButtonSavegame->SetVisible(true);
         m_pImgSavegame->SetVisible(true);

         m_pButtonOptions->GetPos(x,y);
         m_pButtonOptions->SetPos(x,y+(2*dy));
         m_pImgOptions->GetPos(x,y);
         m_pImgOptions->SetPos(x,y+(2*dy)); // Options moves under Save game, so twice as far

         m_pButtonQuit->GetPos(x,y);
         m_pButtonQuit->SetPos(x,y+(2*dy));
         m_pImgQuit->GetPos(x,y);
         m_pImgQuit->SetPos(x,y+(2*dy)); // Leave game moves under Save game, so twice as far

         InGameLayout = true;
      }
      if (!InGame() && InGameLayout)
      {
         PerformDefaultLayout();
         InGameLayout = false;
      }

      // Get mouse coords
      int x,y;
      vgui::input()->GetCursorPos(x,y);

      int fx,fy; // frame xpos, ypos

      GetPos(fx,fy);

      CheckRolloverStartgame(x,y,fx,fy);
      CheckRolloverResumegame(x,y,fx,fy);
      CheckRolloverLoadgame(x,y,fx,fy);
      CheckRolloverSavegame(x,y,fx,fy);
      CheckRolloverOptions(x,y,fx,fy);
      CheckRolloverQuit(x,y,fx,fy);
      
      BaseClass::OnThink();      
   }

   void CheckRolloverStartgame(int x,int y, int fx, int fy)
   {
      int bx,by,bw,bh; // button xpos, ypos, width, height

      m_pButtonStartgame->GetPos(bx,by);
      m_pButtonStartgame->GetSize(bw,bh);

      bx = bx+fx; // xpos for button (rel to screen)
      by = by+fy; // ypos for button (rel to screen)
	  bw = 128;
	  bh = 32;

      // Check and see if mouse cursor is within button bounds
      if ((x > bx && x < bx+bw) && (y > by && y < by+bh))
      {
         if(!InRolloverStartgame) {
            m_pImgStartgame->SetImage("menu_begin_over");
            InRolloverStartgame = true;
         }
      } else {
         if(InRolloverStartgame) {
            m_pImgStartgame->SetImage("menu_begin");
            InRolloverStartgame = false;
         }
      }
   }

   void CheckRolloverResumegame(int x,int y, int fx, int fy)
   {
      if(m_pButtonResumegame->IsVisible()) {
         int bx,by,bw,bh; // button xpos, ypos, width, height

         m_pButtonResumegame->GetPos(bx,by);
         m_pButtonResumegame->GetSize(bw,bh);

         bx = bx+fx; // xpos for button (rel to screen)
         by = by+fy; // ypos for button (rel to screen)

         // Check and see if mouse cursor is within button bounds
         if ((x > bx && x < bx+bw) && (y > by && y < by+bh))
         {
            if(!InRolloverResumegame) {
               m_pImgResumegame->SetImage("menu_Resumegame_over");
               InRolloverResumegame = true;
            }
         } else {
            if(InRolloverResumegame) {
               m_pImgResumegame->SetImage("menu_Resumegame");
               InRolloverResumegame = false;
            }
         }
      }
   }
   void CheckRolloverLoadgame(int x,int y, int fx, int fy)
   {
      int bx,by,bw,bh; // button xpos, ypos, width, height

      m_pButtonLoadgame->GetPos(bx,by);
      m_pButtonLoadgame->GetSize(bw,bh);

      bx = bx+fx; // xpos for button (rel to screen)
      by = by+fy; // ypos for button (rel to screen)

      // Check and see if mouse cursor is within button bounds
      if ((x > bx && x < bx+bw) && (y > by && y < by+bh))
      {
         if(!InRolloverLoadgame) {
            m_pImgLoadgame->SetImage("menu_loadgame_over");
            InRolloverLoadgame = true;
         }
      } else {
         if(InRolloverLoadgame) {
            m_pImgLoadgame->SetImage("menu_loadgame");
            InRolloverLoadgame = false;
         }
      }
   }
   void CheckRolloverSavegame(int x,int y, int fx, int fy)
   {
      if(m_pButtonSavegame->IsVisible()) {
         int bx,by,bw,bh; // button xpos, ypos, width, height

         m_pButtonSavegame->GetPos(bx,by);
         m_pButtonSavegame->GetSize(bw,bh);

         bx = bx+fx; // xpos for button (rel to screen)
         by = by+fy; // ypos for button (rel to screen)

         // Check and see if mouse cursor is within button bounds
         if ((x > bx && x < bx+bw) && (y > by && y < by+bh))
         {
            if(!InRolloverSavegame) {
               m_pImgSavegame->SetImage("menu_Savegame_over");
               InRolloverSavegame = true;
            }
         } else {
            if(InRolloverSavegame) {
               m_pImgSavegame->SetImage("menu_Savegame");
               InRolloverSavegame = false;
            }
         }
      }
   }
   void CheckRolloverOptions(int x,int y, int fx, int fy)
   {
      int bx,by,bw,bh; // button xpos, ypos, width, height

      m_pButtonOptions->GetPos(bx,by);
      m_pButtonOptions->GetSize(bw,bh);

      bx = bx+fx; // xpos for button (rel to screen)
      by = by+fy; // ypos for button (rel to screen)

      // Check and see if mouse cursor is within button bounds
      if ((x > bx && x < bx+bw) && (y > by && y < by+bh))
      {
         if(!InRolloverOptions) {
            m_pImgOptions->SetImage("menu_Options_over");
            InRolloverOptions = true;
         }
      } else {
         if(InRolloverOptions) {
            m_pImgOptions->SetImage("menu_Options");
            InRolloverOptions = false;
         }
      }
   }
   void CheckRolloverQuit(int x,int y, int fx, int fy)
   {
      int bx,by,bw,bh; // button xpos, ypos, width, height

      m_pButtonQuit->GetPos(bx,by);
      m_pButtonQuit->GetSize(bw,bh);

      bx = bx+fx; // xpos for button (rel to screen)
      by = by+fy; // ypos for button (rel to screen)

      // Check and see if mouse cursor is within button bounds
      if ((x > bx && x < bx+bw) && (y > by && y < by+bh))
      {
         if(!InRolloverQuit) {
            m_pImgQuit->SetImage("menu_Quit_over");
            InRolloverQuit = true;
         }
      } else {
         if(InRolloverQuit) {
            m_pImgQuit->SetImage("menu_Quit");
            InRolloverQuit = false;
         }
      }
   }
   bool CMainMenu::InGame()
   {
      C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();

      if(pPlayer && IsVisible())
      {
         return true;
      } else {
         return false;
      }
   }

private:
   vgui::ImagePanel *m_pImgStartgame;
   vgui::ImagePanel *m_pImgResumegame;
   vgui::ImagePanel *m_pImgLoadgame;
   vgui::ImagePanel *m_pImgSavegame;
   vgui::ImagePanel *m_pImgOptions;
   vgui::ImagePanel *m_pImgQuit;
   vgui::Button *m_pButtonStartgame;
   vgui::Button *m_pButtonResumegame;
   vgui::Button *m_pButtonLoadgame;
   vgui::Button *m_pButtonSavegame;
   vgui::Button *m_pButtonOptions;
   vgui::Button *m_pButtonQuit;

   int defaultX;
   int defaultY;
   bool InGameLayout;
   bool InRolloverStartgame;
   bool InRolloverResumegame;
   bool InRolloverLoadgame;
   bool InRolloverSavegame;
   bool InRolloverOptions;
   bool InRolloverQuit;
};

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CMainMenu::CMainMenu( vgui::VPANEL parent ) : BaseClass( NULL, "CMainMenu" )
{
   LoadControlSettings( "resource/UI/MainMenu.res" ); // Optional, don't need this

   SetParent( parent );
   SetTitleBarVisible( false );
   SetMinimizeButtonVisible( false );
   SetMaximizeButtonVisible( false );
   SetCloseButtonVisible( false );
   SetSizeable( true );
   SetMoveable( true );
   SetProportional( true );
   SetVisible( true );
   SetKeyBoardInputEnabled( false );
   SetMouseInputEnabled( false );
   //ActivateBuildMode();
   SetScheme("MenuScheme.res");

        // These coords are relative to a 640x480 screen
        // Good to test in a 1024x768 resolution.
   defaultX = 60; // x-coord for our position
   defaultY = 240; // y-coord for our position
   InGameLayout = false;

   // Size of the panel
   SetSize(512,1024);
   SetZPos(1); // we're behind everything

   // Load invisi buttons
        // Initialize images
   m_pImgStartgame = vgui::SETUP_PANEL(new vgui::ImagePanel(this, "startgame"));
   m_pImgResumegame = vgui::SETUP_PANEL(new vgui::ImagePanel(this, "Resumegame"));
   m_pImgLoadgame = vgui::SETUP_PANEL(new vgui::ImagePanel(this, "Loadgame"));
   m_pImgSavegame = vgui::SETUP_PANEL(new vgui::ImagePanel(this, "Savegame"));
   m_pImgOptions = vgui::SETUP_PANEL(new vgui::ImagePanel(this, "Options"));
   m_pImgQuit = vgui::SETUP_PANEL(new vgui::ImagePanel(this, "Quit"));

   // New game
   
   m_pButtonStartgame = vgui::SETUP_PANEL(new vgui::Button(this, "btnStartgame", ""));   
   m_pButtonStartgame->SetSize(128, 32);
   m_pButtonStartgame->SetPaintBorderEnabled(true);
   m_pButtonStartgame->SetPaintEnabled(false);
   m_pImgStartgame->SetImage("menu_begin");

   // Resume
   m_pButtonResumegame = vgui::SETUP_PANEL(new vgui::Button(this, "btnResumegame", ""));   
   m_pButtonStartgame->SetSize(128, 32);
   m_pButtonResumegame->SetPaintBorderEnabled(false);
   m_pButtonResumegame->SetPaintEnabled(false);
   m_pImgResumegame->SetImage("menu_resumegame");

   // Load
   m_pButtonLoadgame = vgui::SETUP_PANEL(new vgui::Button(this, "btnLoadgame", ""));
   m_pButtonStartgame->SetSize(128, 32);
   m_pButtonLoadgame->SetPaintBorderEnabled(false);
   m_pButtonLoadgame->SetPaintEnabled(false);
   m_pImgLoadgame->SetImage("menu_loadgame");

   // Save
   m_pButtonSavegame = vgui::SETUP_PANEL(new vgui::Button(this, "btnSavegame", ""));
   m_pButtonStartgame->SetSize(128, 32);
   m_pButtonSavegame->SetPaintBorderEnabled(false);
   m_pButtonSavegame->SetPaintEnabled(false);
   m_pImgSavegame->SetImage("menu_savegame");

   // Options
   m_pButtonOptions = vgui::SETUP_PANEL(new vgui::Button(this, "btnOptions", ""));
   m_pButtonStartgame->SetSize(128, 32);
   m_pButtonOptions->SetPaintBorderEnabled(false);
   m_pButtonOptions->SetPaintEnabled(false);
   m_pImgOptions->SetImage("menu_options");

   // Quit
   m_pButtonQuit = vgui::SETUP_PANEL(new vgui::Button(this, "btnQuit", ""));
   m_pButtonStartgame->SetSize(128, 32);
   m_pButtonQuit->SetPaintBorderEnabled(false);
   m_pButtonQuit->SetPaintEnabled(false);
   m_pImgQuit->SetImage("menu_quit");

   PerformDefaultLayout();
}

void CMainMenu::OnCommand(const char *command)
{

   BaseClass::OnCommand(command);
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CMainMenu::~CMainMenu()
{
}

// Class
// Change CSMenu to CModMenu if you want. Salient is the name of the source mod,
// hence SMenu. If you change CSMenu, change ISMenu too where they all appear.
class CSMenu : public ISMenu
{
private:
   CMainMenu *MainMenu;
   vgui::VPANEL m_hParent;

public:
   CSMenu( void )
   {
      MainMenu = NULL;
   }

   void Create( vgui::VPANEL parent )
   {
      // Create immediately
      MainMenu = new CMainMenu(parent);
   }

   void Destroy( void )
   {
      if ( MainMenu )
      {
         MainMenu->SetParent( (vgui::Panel *)NULL );
         delete MainMenu;
      }
   }

};

static CSMenu g_SMenu;
ISMenu *SMenu = ( ISMenu * )&g_SMenu;