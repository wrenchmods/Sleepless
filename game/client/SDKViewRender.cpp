#include "cbase.h"
#include "viewrender.h"
#include "view_scene.h"
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"
 
class C_SDKViewRender : public CViewRender {
	DECLARE_CLASS(C_SDKViewRender, CViewRender);
public:
	C_SDKViewRender::C_SDKViewRender();
 
	virtual void RenderView(const CViewSetup &view, int nClearFlags, int whatToDraw);
};
 
static C_SDKViewRender g_ViewRender;
 
//=================================================================================
 
C_SDKViewRender::C_SDKViewRender() {
	if (!view) {
		view = (IViewRender *)&g_ViewRender;
	}
}
 
void C_SDKViewRender::RenderView(const CViewSetup &view, int nClearFlags, int whatToDraw) {
	BaseClass::RenderView(view, nClearFlags, whatToDraw);
	//Your code could go here.
}