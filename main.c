#include <nusys.h>
#include "stages.h"
#include "config.h"

static void vsyncCallback(int pendingTaskCount);

void mainproc(void * dummy){
	nuGfxInit();
	stage00_init();
	nuGfxFuncSet((NUGfxFunc)vsyncCallback);
	nuGfxDisplayOn();
	while(1);
}

void vsyncCallback(int pendingTaskCount)
{
	stage00_update();
	if (pendingTaskCount < 1)
		stage00_draw();
}