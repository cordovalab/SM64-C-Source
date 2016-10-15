#include <stdint.h>
#include "includes.h"

uint32_t bss0[0x16770] = {0};
uint8_t spIdle[0x800], spMain[0x2000], sp4[0x2000], sp5[0x2000];
osThread * idleThread, * mainThread, * Thread4, * Thread5;
uint32_t idleThread_cond0 = 0;
uint8_t hasMainThreadBeenStarted = 0;
osMesgQueue msgQueue1, mQS1, mQS2, mQS3;
OsMesg msg1, mS1, mS2, mS3;

void CreateThread(OSThread *t, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri)
{
	t->next = NULL;
	t->queue = NULL;
	osCreateThread(t, id, entry, arg, sp, pri);
}

void SetupMessageQueues()
{
	osCreateMesgQueue(&mQS1,mS1,1);
	osCreateMesgQueue(&mQS2,mS2,1);
	osSetEventMesg(OS_EVENT_SI,mS2,NULL);
	osCreateMesgQueue(&mQS3,mS3,0x10);
	osCreateMesgQueue(&msgQueue1,msg1,0x10);
	osViSetEventMsg(&msgQueue1,102,1);
	osSetEventMesg(OS_EVENT_SP,msg1,100);
	osSetEventMesg(OS_EVENT_DP,msg1,101);
	osSetEventMesg(OS_EVENT_PRENMI,msg1,104);
}

uint32_t AllocPool()
{
	//TODO
	InitMemPool(0x8005C000,0x801C1000);
	proc_80278B98(0x4000,0);
	return (uint32_t*)0x8033B494;
}

void Thread4()
{
	InitAudioSystem();
}

void Thread5_Debug()
{
	
}

void Thread3_Main(void * arg)
{
	int msg;
	SetupMessageQueues();
	AllocPool();
	CopyScriptInterpreter();
	CreateThread(Thread4,4,Thread4(),NULL,sp4+0x2000,20);
	osStartThread(Thread4);
	CreateThread(Thread5,5,Thread5_Debug(),NULL,sp5+0x2000,20);
	osStartThread(Thread5);
	while(1)
	{
		uint32_t check = osRecvMesg(&msgQueue,&msg,OS_MESG_BLOCK);
		switch msg
		{
			case 100:
			{
				proc_802467FC();
				break;
			}
			case 101:
			{
				proc_8024694C();
				break;
			}
			case 102:
			{
				proc_8024669C();
				break;
			}
			case 103:
			{
				KickTask();
				break;
			}
			case 104:
			{
				proc_8024639C();
				break;
			}
		}
	}
}

void Thread1_Idle(void * arg)
{
	osCreateViManager(OS_PRIORITY_VIMGR);
	if(idleThread_cond0==1) osViSetMode(OS_VI_NTSC_LAN1);
	else osViSetMode(OS_VI_MPAL_LAN1);
	osViBlack(1);
	osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON | OS_VI_GAMMA_OFF);
	osCreatePiManager(150,0x8033ADF0,0x8033AE40,32);
	CreateThread(mainThread,3,Thread3_Main(),NULL,spMain+0x2000,100);
	if(hasMainThreadBeenStarted==0) osStartThread(mainThread);
	osSetThreadPri(0,0);
	while(1);
}

void MainFunc()
{
	osInitialize();
	CreateThread(idleThread,1,Thread1_Idle(),NULL,spIdle+0x800,100);
	osStartThread(idleThread);
}

void boot()
{
	MainFunc();
}
