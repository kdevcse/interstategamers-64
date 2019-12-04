#include <nusys.h>
#include "../../config.h"
#include "definitions.h"
#include "../sprites/dGasm.h"
#include "../sprites/dOof.h"
#include "../sprites/kappaRide.h"

static u8 r;
static u8 g;
static u8 b;
static int scene;

static void DrawScene();

void stage01_init(void)
{
	scene = 0;
}

void stage01_update(void)
{
	nuContDataGetExAll(contData);
}

void stage01_draw(void)
{
	char r_str[3];
	char g_str[3];
	char b_str[3];
	
	// Initialize RCP
	glistp = glist;
	RCPInit(glistp);
	
	// Draw
	DrawScene();

	// Sync
	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
	nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx),
		NU_GFX_UCODE_F3DEX, NU_SC_SWAPBUFFER);
		
	//Debug commands
	if (scene == -1) {
		nuDebConClear(NU_DEB_CON_WINDOW0);
		nuDebConCPuts(NU_DEB_CON_WINDOW0, "Debug Menu\n\n");
		nuDebConDisp(NU_SC_SWAPBUFFER);
	}

}

static void DrawScene(){
	switch(scene){
		case -1:
			ClearBackground(0, 0, 0);
			break;
		case 0:
			ClearBackground(DEF_BG_R, DEF_BG_G, DEF_BG_B);
			break;
		case 1:
			ClearBackground(DEF_BG_R, DEF_BG_G, DEF_BG_B);
			DrawBigImg32(TITLE_X_POS, TITLE_Y_POS, 112, 112, dOof);
			break;
		case 2:
			ClearBackground(DEF_BG_R, DEF_BG_G, DEF_BG_B);
			DrawBigImg32(TITLE_X_POS, TITLE_Y_POS, 116, 128, kappaRide);
			break;
		default:
			ClearBackground(DEF_BG_R, DEF_BG_G, DEF_BG_B);
			DrawBigImg32(TITLE_X_POS, TITLE_Y_POS, 112, 107, dGasm);
			scene = 0;
			break;
	}
}