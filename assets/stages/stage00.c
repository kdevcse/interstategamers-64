#include <nusys.h>
#include "../../config.h"
#include "../sprites/IGLogo32.h"
#include "../sprites/IGMainWhite200.h"
#include "definitions.h"

static u8 r;
static u8 g;
static u8 b;
static int pos_y;
static int scene;
static u8 gasm;

static void DrawScene();

void DrawIcon(int x, int y, float scale_x, float scale_y);

void stage00_init(void)
{
	scene = 0;
	gasm = 0;
	pos_y = ICON_DEFAULT_Y_POS;
	
	//Initialize Console
	nuDebConWindowPos(0, 25, 25);
}

void stage00_update(void)
{
	nuContDataGetExAll(contData);
	
	if(contData[0].stick_y < 0)
		pos_y = ICON_DEFAULT_Y_POS + MENU_OFFSET_Y;
	else if (contData[0].stick_y > 0)
		pos_y = ICON_DEFAULT_Y_POS;
	
	// Set debug mode
	if(contData[0].trigger & START_BUTTON){
		if (scene > -1)
			scene = -1;
		else
			scene = 0;
	}
	
	if(contData[0].trigger & A_BUTTON){
		if (scene > -1)
			scene++;
	}
}

void stage00_draw(void)
{
	char r_str[3];
	char g_str[3];
	char b_str[3];
		char info[50];
	
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
		sprintf(info, "Stage: %d", current_stage);
		nuDebConCPuts(NU_DEB_CON_WINDOW0, info);
		nuDebConDisp(NU_SC_SWAPBUFFER);
	}

}

void DrawIcon(int x, int y, float scale_x, float scale_y)
{
    float w = 32*scale_x;
    float h = 32*scale_y;
    s32 sx = (int) ((1<<10) / scale_x + 0.5F);
    s32 sy = (int) ((1<<10) / scale_y + 0.5F);
	
    gDPSetCycleType(glistp++, G_CYC_1CYCLE);
    gDPSetCombineMode(glistp++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    gDPSetRenderMode(glistp++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE);
    gDPSetDepthSource(glistp++, G_ZS_PRIM);
    gDPSetPrimDepth(glistp++, 0, 0);
    gDPSetTexturePersp(glistp++, G_TP_NONE);
	
    gDPLoadTextureBlock(glistp++, 
        IGLogo32, 
        G_IM_FMT_RGBA, G_IM_SIZ_32b, 
        32, 32, 0, 
        G_TX_WRAP, G_TX_WRAP, 
        5, 5, 
        G_TX_NOLOD, G_TX_NOLOD);
    gSPTextureRectangle(glistp++, 
		((int)(x-w/2))<<2, ((int)(y-h/2))<<2,
		((int)(x+w/2))<<2, ((int)(y+h/2))<<2, 
        G_TX_RENDERTILE, 
        0 << 5, 0 << 5, 
        sx, sy);
    gDPPipeSync(glistp++);
}

static void DrawScene(){
	switch(scene){
		case -1:
			ClearBackground(0, 0, 0);
			break;
		case 0:
			ClearBackground(DEF_BG_R, DEF_BG_G, DEF_BG_B);
			DrawBigImg32(TITLE_X_POS, TITLE_Y_POS, 200, 66, IGMainWhite200);
			DrawText(MENU_START_TXT_POS_X, MENU_START_TXT_POS_Y, "START", 
				TEXT_HALIGN_LEFT, TEXT_VALIGN_TOP);
			DrawText(MENU_OPTIONS_TXT_POS_X, MENU_OPTIONS_TXT_POS_Y, "OPTIONS", 
				TEXT_HALIGN_LEFT, TEXT_VALIGN_TOP);
			DrawIcon(ICON_DEFAULT_X_POS, pos_y, 0.5, 0.5);
			break;
		default:
			ClearBackground(DEF_BG_R, DEF_BG_G, DEF_BG_B);
			DrawBigImg32(TITLE_X_POS, TITLE_Y_POS, 200, 66, IGMainWhite200);
			DrawIcon(ICON_DEFAULT_X_POS, pos_y, 0.5, 0.5);
			scene = 0;
			current_stage = 1;
	}
}