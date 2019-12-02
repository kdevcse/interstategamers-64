#include <nusys.h>
#include "../../config.h"
#include "../sprites/IGLogo32.h"
#include "definitions.h"

static u8 color;
static u8 r;
static u8 g;
static u8 b;
static int x;
static int y;
static float hover;

void DrawLogo(int x, int y);

void ClearBackground(u8 r, u8 g, u8 b);

void stage00_init(void)
{
	color = 0;
	r = 0;
	g = 0;
	b = 255;
	x = 160;
	y = 120;
	hover = 0;
}

void stage00_update(void)
{
	nuContDataGetExAll(contData);
	
	x = 160 + contData[0].stick_x;
	y = 120 - contData[0].stick_y;
	
	if ((contData[0].trigger & A_BUTTON))
	{
		switch(color){
			case 0:
				r = 0;
				g = 255;
				b = 0;
				break;
			case 1:
				r = 255;
				g = 0;
				b = 0;
				break;
			case 2:
				r = 0;
				g = 0;
				b = 255;
				break;
			case 3:
				r = 255;
				g = 255;
				b = 255;
				break;
			default:
				r = 0;
				g = 0;
				b = 0;
				break;
		}
		
		if(color > 2)
			color = 0;
		else
			color += 1;
	}
	
	if (contData[0].trigger & B_BUTTON)
	{
		r = 0;
		g = 0;
		b = 255;
	}
}

void stage00_draw(void)
{
	// Initialize RCP
	glistp = glist;
	RCPInit(glistp);
	
	ClearBackground(r, g, b);
	DrawLogo(x,y);
	
	gDPFullSync(glistp++);
    gSPEndDisplayList(glistp++);
    nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx),
                   NU_GFX_UCODE_F3DEX, NU_SC_SWAPBUFFER);

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

void DrawLogo(int x, int y)
{
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
        x-16<<2, y-16<<2, 
        (x + 16)<<2, (y + 16)<<2,
        G_TX_RENDERTILE, 
        0 << 5, 0 << 5, 
        1 << 10, 1 << 10);
    gDPPipeSync(glistp++);
}