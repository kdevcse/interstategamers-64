#include <nusys.h>
#include "config.h"

#include "assets/sprites/fnt_text.h"
#include "assets/sprites/fnt_text_outlined.h"
#include "assets/sprites/fnt_numbers.h"
#include "assets/sprites/fnt_numbers_outlined.h"

static unsigned char* global_text_sprite = fnt_text;
static char global_text_alpha = 255;
static unsigned short* global_text_tlut = fnt_text_tlut;

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

void DrawText(int x, int y, char* text, char halign, char valign)
{
    int i=0, j=0, posx=-1, posy=0;
    char newline_total = 0;
    char current_line = 0;
    int string_length = strlen(text);
    char TEXT_W = 8;
    char TEXT_H = 8;
    short TEXT_HPAD = 0;
    short TEXT_VPAD = 2;

    // Change some settings if using an outlined text
    if (global_text_sprite == fnt_text_outlined)
    {
        TEXT_H = 10;
        TEXT_VPAD = 0;
        TEXT_HPAD = 0;
    }
    
    // Count the number of newlines
    for (i=0;i<string_length;i++)
        if (text[i] == '\n')
            newline_total++;
            
            
    // Calculate the vertical alignment based on the number of newlines
    if (valign == TEXT_VALIGN_MIDDLE)
        posy -= (newline_total*TEXT_H)/2 + TEXT_H/2  + (TEXT_VPAD*newline_total)/2;
    else if (valign == TEXT_VALIGN_BOTTOM)
        posy -= (newline_total*TEXT_H) + TEXT_H + (TEXT_VPAD*newline_total);
    
    
    // Set drawing mode
    gDPSetCycleType(glistp++, G_CYC_1CYCLE);
    gDPSetCombineMode(glistp++, G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM );
    gDPSetRenderMode(glistp++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
    gDPSetDepthSource(glistp++, G_ZS_PRIM);
    gDPSetTexturePersp(glistp++, G_TP_NONE);
    gDPSetTextureLUT(glistp++, G_TT_RGBA16 );
    gDPSetPrimDepth(glistp++, 10, 0);
    gDPSetPrimColor(glistp++, 0, 0, 255, 255, 255, global_text_alpha);
    
    gDPLoadTLUT_pal16(glistp++, 0, global_text_tlut);
    
    
    // Draw the text
    for (i=0;i<string_length;i++)
    {
        char offsetx;
        float offsety;
        
        
        // If on a newline, calculate the horizontal alignment
        if (posx == -1)
        {
            posx = 0;
            if (halign != TEXT_HALIGN_LEFT)
            {
                char chars_to_newline = 0;
                char current_newline = 0;
                
                for (j=0;j<string_length;j++)
                {
                    if (current_newline == current_line)
                        break;
                    if (text[j] == '\n')
                        current_newline++;
                }
                
                for (;j<string_length;j++)
                {
                    if (text[j] == '\n')
                        break;
                    else
                        chars_to_newline++;
                }
                if (halign == TEXT_HALIGN_RIGHT)
                    posx -= (chars_to_newline*TEXT_W)+TEXT_HPAD*chars_to_newline;
                else
                    posx -= ((chars_to_newline)*TEXT_W)/2 + ((chars_to_newline)*TEXT_HPAD)/2;
            }
        }
        
        
        // If we found a new line, move the text position
        if (text[i] == '\n')
        {
            posy+=TEXT_H+TEXT_VPAD;
            posx=-1;
            current_line++;
            continue;
        }
        
        // Don't render if we're out of the screen
        if (x+posx+TEXT_W+TEXT_HPAD > SCREEN_WD)
            continue;
        
        // Offset the font sprite based on what character we're currently drawing.
        if (text[i] >= '0' && text[i] <= '9' && (global_text_sprite == fnt_text || global_text_sprite == fnt_text_outlined))
        {
            char * sprite;
            if (global_text_sprite == fnt_text)
                sprite = fnt_numbers;
            else
                sprite = fnt_numbers_outlined;
            offsety = text[i] - '0';
            gDPLoadTextureTile_4b(glistp++, sprite, G_IM_FMT_CI, TEXT_W, TEXT_H, 0, (int)(offsety*TEXT_H), TEXT_W, TEXT_H + (int)(offsety*TEXT_H), 0, G_TX_WRAP, G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            gSPScisTextureRectangle(glistp++, x+posx << 2, y+posy << 2, x+TEXT_W+posx << 2, y+posy+TEXT_H << 2,  G_TX_RENDERTILE,  0 << 5, (int)offsety*TEXT_H << 5,  1 << 10, 1 << 10);
        }
        else
        {
            if (text[i] >= 'a' && text[i] <= 'z')
                text[i] = text[i] + ('A' - 'a');
                
            if (text[i] >= 'A' && text[i] <= 'P')
            {
                offsetx = text[i] - 'A';
                offsety = 0;
            }
            else if (text[i] >= 'Q' && text[i] <= 'Z')
            {
                if (global_text_sprite == fnt_text_outlined)
                    offsety = 0.9;
                else
                    offsety = 0.875; // To fix a hardware rendering bug. Change to 1 to melt your brain
                offsetx = text[i] - 'A';
            }
            else
            {
                switch (text[i])
                {
                    case ',': offsety = 1; offsetx = 10; break;
                    case '.': offsety = 1; offsetx = 11; break;
                    case '!': offsety = 1; offsetx = 12; break;
                    case '?': offsety = 1; offsetx = 13; break;
                    case '\'': offsety = 1; offsetx = 14; break;
                    case '-': offsety = 1; offsetx = 15; break;
                    case ':': offsety = 2; offsetx = 0; break;
                    case '\a': offsety = 2; offsetx = 1; break;
                    case '[': offsety = 2; offsetx = 2; break;
                    case ']': offsety = 2; offsetx = 3; break;
                    case '(': offsety = 2; offsetx = 4; break;
                    case ')': offsety = 2; offsetx = 5; break;
                    case '{': offsety = 2; offsetx = 6; break;
                    case '}': offsety = 2; offsetx = 7; break;
                    default: posx+=TEXT_W+TEXT_HPAD; continue;
                }
            }
            
            gDPLoadTextureTile_4b(glistp++, global_text_sprite, G_IM_FMT_CI, 128, 32, offsetx*TEXT_W, (int)(offsety*TEXT_H), TEXT_W + (offsetx*TEXT_W), TEXT_H + (int)(offsety*TEXT_H), 0, G_TX_WRAP, G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            gSPScisTextureRectangle(glistp++, x+posx << 2, y+posy << 2, x+posx+TEXT_W << 2, y+posy+TEXT_H << 2,  G_TX_RENDERTILE,  offsetx*TEXT_W << 5, (int)(offsety*TEXT_H) << 5,  1 << 10, 1 << 10);
        }
        posx+=TEXT_W+TEXT_HPAD;
    }
    gDPPipeSync(glistp++);
}