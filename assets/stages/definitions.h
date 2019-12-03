#ifndef N64_STAGES_H
#define N64_STAGES_H

	/* Title img positions */
	#define TITLE_X_POS   50
	#define TITLE_Y_POS   60
	
	/* IG icon positions */
	#define ICON_START_X_POS	83
	#define ICON_START_Y_POS	160
	
	/* Default Background Color Values (Official IG Blue) */
	#define DEF_BG_R	45
	#define DEF_BG_G	50
	#define DEF_BG_B	175
 
	void stage00_init();
	void stage00_update();
	void stage00_draw();
 
#endif
