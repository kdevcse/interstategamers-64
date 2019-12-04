#ifndef N64_STAGES_H
#define N64_STAGES_H

	/* Title img positions */
	#define TITLE_X_POS   50
	#define TITLE_Y_POS   60
	
	/* IG icon positions */
	#define ICON_DEFAULT_X_POS	125
	#define ICON_DEFAULT_Y_POS	160
	
	/* Menu txt positions */
	#define MENU_OFFSET_Y		30
	#define MENU_START_TXT_POS_X	140
	#define MENU_START_TXT_POS_Y	157
	#define MENU_OPTIONS_TXT_POS_X	140
	#define MENU_OPTIONS_TXT_POS_Y	187
	
	/* Default Background Color Values (Official IG Blue) */
	#define DEF_BG_R	45
	#define DEF_BG_G	50
	#define DEF_BG_B	175
 
	void stage00_init();
	void stage00_update();
	void stage00_draw();
 
#endif
