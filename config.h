#ifndef N64_CONFIG_H
#define N64_CONFIG_H
 
	#define SCREEN_WD   320
	#define SCREEN_HT   240
 
	#define GLIST_LENGTH	2048
	#define CheckController(cont) (contPattern & (1<<(cont)))
	
	/* Defintions */
	
	Gfx glist[GLIST_LENGTH];
	Gfx* glistp;
	extern NUContData contData[1];
	extern u8 contPattern;
	
	/* Functions */
	void RCPInit(Gfx * glistp);
	
#endif
