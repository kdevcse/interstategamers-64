#ifndef N64_CONFIG_H
#define N64_CONFIG_H
 
	#define SCREEN_WD   320
	#define SCREEN_HT   240
 
	#define GLIST_LENGTH	2048
	#define CheckController(cont) (contPattern & (1<<(cont)))
	
    // Text defines
    #define TEXT_HALIGN_LEFT 0
    #define TEXT_HALIGN_CENTER 1
    #define TEXT_HALIGN_RIGHT 2
    #define TEXT_VALIGN_TOP 0
    #define TEXT_VALIGN_MIDDLE 1
    #define TEXT_VALIGN_BOTTOM 2
	
	/* Defintions */
	
	Gfx glist[GLIST_LENGTH];
	Gfx* glistp;
	extern NUContData contData[1];
	extern u8 contPattern;
	extern int current_stage;
	
	/* Functions */
	void RCPInit(Gfx * glistp);
	
	/* Helper Functions */
	void DrawText(int x, int y, char* text, char halign, char valign);
	void ClearBackground(u8 r, u8 g, u8 b);
	void DrawBigImg32(int x, int y, int w, int h, unsigned int img[]);
	
#endif
