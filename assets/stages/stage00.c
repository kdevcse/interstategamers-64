#include <nusys.h>
#include "../../config.h"
#include "../sprites/IGLogo32.h"
#include "../sprites/IGMainWhite200.h"
#include "../sprites/dGasm.h"
#include "../sprites/dOof.h"
#include "../sprites/kappaRide.h"
#include "definitions.h"

static u8 r;
static u8 g;
static u8 b;
static int pos_y;
static int scene;
static u8 gasm;

void DrawBigImg32(int x, int y, int w, int h, unsigned int img[]);

void DrawLogo(int x, int y, float scale_x, float scale_y);

void ClearBackground(u8 r, u8 g, u8 b);

void DrawScene();

void stage00_init(void)
{
	scene = 0;
	gasm = 0;
	pos_y = ICON_START_Y_POS;
	
	//Initialize Console
	nuDebConWindowPos(0, 25, 25);
}

void stage00_update(void)
{
	nuContDataGetExAll(contData);
	
	if(contData[0].stick_y < 0)
		pos_y = ICON_START_Y_POS + 35;
	else if (contData[0].stick_y > 0)
		pos_y = ICON_START_Y_POS;
	
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

void ClearBackground(u8 r, u8 g, u8 b)
{
	gDPPipeSync(glistp++);
    gDPSetCycleType(glistp++, G_CYC_FILL);
    gDPSetDepthImage(glistp++, nuGfxZBuffer);
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, nuGfxZBuffer);
    gDPSetFillColor(glistp++, (GPACK_ZDZ(G_MAXFBZ, 0) << 16 | GPACK_ZDZ(G_MAXFBZ, 0)));
    gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
    gDPPipeSync(glistp++);
	
    gDPSetCycleType(glistp++, G_CYC_FILL);
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
                     osVirtualToPhysical(nuGfxCfb_ptr));
    gDPSetFillColor(glistp++, (GPACK_RGBA5551(r, g, b, 1) << 16 |
                               GPACK_RGBA5551(r, g, b, 1)));
    gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
    gDPPipeSync(glistp++);
}

void DrawBigImg32(int x, int y, int w, int h, unsigned int img[]){
	int i = 0;
	
    gDPSetCycleType(glistp++, G_CYC_1CYCLE);
    gDPSetCombineMode(glistp++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    gDPSetRenderMode(glistp++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE);
    gDPSetDepthSource(glistp++, G_ZS_PRIM);
    gDPSetPrimDepth(glistp++, 0, 0);
    gDPSetTexturePersp(glistp++, G_TP_NONE);
	
	for (i=0; i<h; i++)
	{
		gDPLoadMultiTile(
			glistp++,
			img,          			  // timg - Our sprite array
			0,                        // tmem - Address to store in TMEM
			G_TX_RENDERTILE,          // rt - Tile descriptor
			G_IM_FMT_RGBA,            // fmt - Our image format
			G_IM_SIZ_32b,             // size - Pixel size
			w, h,                     // width, height of the full image
			0, i,                     // Top left corner of the rectangle
			w-1, i,                   // Bottom right corner
			0,                        // Palette to use (always 0)
			G_TX_WRAP, G_TX_WRAP,     // cms, cmt
			G_TX_NOMASK, G_TX_NOMASK, // masks, maskt
			G_TX_NOLOD, G_TX_NOLOD    // shifts, shiftt
		);
			
		gSPTextureRectangle(glistp++, 
			x << 2, y + i << 2, 
			x + w << 2, y + i + 1 << 2,
			G_TX_RENDERTILE, 
			0 << 5, 0 << 5, 
			1 << 10, 1 << 10);
		
		gDPPipeSync(glistp++);
	}
}

void DrawLogo(int x, int y, float scale_x, float scale_y)
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
        G_TX_NOMASK, G_TX_NOMASK, 
        G_TX_NOLOD, G_TX_NOLOD);
    gSPTextureRectangle(glistp++, 
		((int)(x-w/2))<<2, ((int)(y-h/2))<<2,
		((int)(x+w/2))<<2, ((int)(y+h/2))<<2, 
        G_TX_RENDERTILE, 
        0 << 5, 0 << 5, 
        sx, sy);
    gDPPipeSync(glistp++);
}

void DrawScene(){
	switch(scene){
		case -1:
			ClearBackground(0, 0, 0);
			break;
		case 0:
			ClearBackground(DEF_BG_R, DEF_BG_G, DEF_BG_B);
			DrawBigImg32(TITLE_X_POS, TITLE_Y_POS, 200, 66, IGMainWhite200);
			DrawLogo(ICON_START_X_POS, pos_y, 1, 1);
			break;
		case 1:
			ClearBackground(DEF_BG_R, DEF_BG_G, DEF_BG_B);
			DrawBigImg32(TITLE_X_POS, TITLE_Y_POS, 112, 107, dGasm);
			break;
		case 2:
			ClearBackground(DEF_BG_R, DEF_BG_G, DEF_BG_B);
			DrawBigImg32(TITLE_X_POS, TITLE_Y_POS, 112, 112, dOof);
			break;
		case 3:
			ClearBackground(DEF_BG_R, DEF_BG_G, DEF_BG_B);
			DrawBigImg32(TITLE_X_POS, TITLE_Y_POS, 116, 128, kappaRide);
			break;
		default:
			ClearBackground(DEF_BG_R, DEF_BG_G, DEF_BG_B);
			DrawBigImg32(TITLE_X_POS, TITLE_Y_POS, 200, 66, IGMainWhite200);
			DrawLogo(ICON_START_X_POS, pos_y, 1, 1);
			scene = 0;
	}
}