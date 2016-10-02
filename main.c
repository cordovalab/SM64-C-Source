#include <stdint.h>
#include "includes.h"

uint32_t bss0[0x16770] = {0}; //0x8033A580
uint32_t spIdle = 0x80200600, spMain = 0x80200E00, sp4 = 0x80202E00, sp5 = 0x80204E00;
osThread * idleThread, * mainThread, * Thread4, * Thread5; //0x80246CF0, 0x802469B8, 0x8033AC40, 0x8033AA90
uint32_t idleThread_cond0 = 0;
uint8_t hasMainThreadBeenStarted = 0; //0x8032D590

void Thread3_Main()
{
	SetupMessageQueues();
	AllocPool();
	CopyScriptInterpreter();
	osCreateThread(Thread4,4,Thread4(),NULL,sp4+0x2000,20);
	osStartThread(Thread4);
	osCreateThread(Thread5,5,Thread5(),NULL,sp5+0x2000,20);
	osStartThread(Thread5);
	osRecvMesg(Thread5,36,1);
	//left off here
}

void Thread1_Idle()
{
	osCreateViManager(OS_PRIORITY_VIMGR);
	//TODO Find out what these modes are (look at RAM perhaps)
	if(idleThread_cond0==1) osViSetMode((*(uint32_t*)(0x80335010))+0xA0);
	else osViSetMode((*(uint32_t*)(0x80335010))+0x500);
	//end TODO block
	osViBlack(1);
	osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON | OS_VI_GAMMA_OFF);
	osCreatePiManager(150,0x8033ADF0,0x8033AE40,32);
	osCreateThread(mainThread,3,Thread3_Main(),NULL,spMain+0x2000,100);
	if(hasMainThreadBeenStarted==0) osStartThread(mainThread);
	osSetThreadPri(0,0);
	while(1);
}

void MainFunc()
{
	osInitialize();
	// Dummy routine was here
	osCreateThread(idleThread,1,Thread1_Idle(),NULL,spIdle+0x800,100);
	osStartThread(idleThread);
	if(
}

void main() // EntryPoint
{
	// Jump to our actual "Main" function
	MainFunc();
}
