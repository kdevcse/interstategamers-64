#include <nusys.h>
#include "assets/stages/definitions.h"
#include "config.h"

static void vsyncTitleScreenCallback(int pendingTaskCount);
static void vsyncGameCallback(int pendingTaskCount);
NUContData contData[1];
u8 contPattern;

volatile short int current_stage;

void mainproc(void * dummy){
	nuGfxInit();
	contPattern = nuContInit();
	
	current_stage = STAGE_TITLE;
	
	while(1){
		switch(current_stage){
			case STAGE_TITLE:
				stage00_init();
				nuGfxFuncSet((NUGfxFunc)vsyncTitleScreenCallback);
				nuGfxDisplayOn();
				current_stage = -1;
				break;
			case STAGE_GAME:
				stage01_init();
				nuGfxFuncSet((NUGfxFunc)vsyncTitleScreenCallback);
				nuGfxDisplayOn();
				current_stage = -1;
				break;
			default:
				break;
		}
		
		while(current_stage == -1);
	}
}

void vsyncTitleScreenCallback(int pendingTaskCount)
{
	stage00_update();
	if (current_stage == -1 & pendingTaskCount < 1)
		stage00_draw();
}

void vsyncGameCallback(int pendingTaskCount)
{
	stage01_update();
	if (current_stage == -1 & pendingTaskCount < 1)
		stage01_draw();
}