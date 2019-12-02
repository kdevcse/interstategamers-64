#ifndef N64_CONFIG_H
#define N64_CONFIG_H
 
	#define SCREEN_WD   320
	#define SCREEN_HT   240
 
	#define GLIST_LENGTH	2048
	
	/* Defintions */
	
	Gfx glist[GLIST_LENGTH];
	Gfx* glistp;
	
	/* Functions */
	void RCPInit(Gfx * glistp);
	
#endif
