#include <nusys.h>
#include "../../config.h"
#include "definitions.h"
#include "table.h"

static u8 r;
static u8 g;
static u8 b;
static int scene;
static int selected_game;

static void DrawScene();
static void DrawTable();
static int GetColumnOffset(int column);

void stage01_init(void)
{
	scene = 0;
	selected_game = 1;
}

void stage01_update(void)
{
	nuContDataGetExAll(contData);
	
	//Exit back to menu
	if (contData[0].trigger & B_BUTTON){
		current_stage = 0;
		return;
	}
	
	// Set debug mode
	if (contData[0].trigger & START_BUTTON){
		if (scene > -1)
			scene = -1;
		else
			scene = 0;
	}
	
	if (contData[0].trigger & U_JPAD){
		if (selected_game > 1)
			selected_game--;
	}
	
	if (contData[0].trigger & D_JPAD){
		if (selected_game < NUM_OF_ROWS - 1)
			selected_game++;
	}
}

void stage01_draw(void)
{
	char r_str[3];
	char g_str[3];
	char b_str[3];
	char info[25];
	
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
		sprintf(info, "selected_game: %d\nGame Length: %d",selected_game,strlen(table[selected_game][1]));
		nuDebConCPuts(NU_DEB_CON_WINDOW0, info);
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
			DrawText(
				(int)((240 - strlen(table[selected_game][1]) * 4.25)/2),
				30,
				table[selected_game][1],
				TEXT_HALIGN_LEFT, TEXT_VALIGN_TOP
			);
			DrawTable();
			break;
		default:
			ClearBackground(DEF_BG_R, DEF_BG_G, DEF_BG_B);
			DrawTable();
			scene = 0;
			break;
	}
}

static void DrawTable(){
	int i, j;
	char game[30];
	for(j = 0; j < NUM_OF_COLUMNS; j++) {
		
		//Draw Header
		DrawText(
			TABLE_LEFT_OFFSET + GetColumnOffset(j),
			TABLE_TOP_OFFSET,
			table[0][j],
			TEXT_HALIGN_LEFT, TEXT_VALIGN_TOP
		);
		
		for(i = 0; i < MAX_GAMES_ON_PAGE; i++){
			//Ensure we don't draw outside of the screen
			if(TABLE_TOP_OFFSET + ((i + 1) * ROW_HEIGHT) >= SCREEN_HT)
				break;
			
			//Ensure we don't get an array out of bounds
			if (selected_game + i >= NUM_OF_ROWS)
				break;
			
			sprintf(game, "%s", table[selected_game + i][j]);
			
			//Word break algorithm
			if (strlen(game) > WORD_BREAK_POS)
				BreakWord(game, WORD_BREAK_POS);
					
			//Draw Row
			DrawText(
				TABLE_LEFT_OFFSET + GetColumnOffset(j),
				TABLE_TOP_OFFSET + ((i + 1) * ROW_HEIGHT),
				game,
				TEXT_HALIGN_LEFT, TEXT_VALIGN_TOP
			);
		}
	}
}

static int GetColumnOffset(int column){
	switch(column){
		case 0:
			return 0;
		case 1:
			return TITLE_OFFSET;
		case 2:
			return SCORE_OFFSET;
		default:
			return COLUMN_WIDTH;
	}
}