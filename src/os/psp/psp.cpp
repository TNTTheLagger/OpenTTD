#ifdef PSP

#include "../../stdafx.h"
#include <pspkernel.h>
#include <pspdebug.h>
#include <psppower.h>
#include <pspctrl.h>

PSP_MODULE_INFO("OpenTTD", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-256);

static int exit_callback(int arg1, int arg2, void *common)
{
	sceKernelExitGame();
	return 0;
}

static int CallbackThread(SceSize args, void *argp)
{
	int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, nullptr);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

int SetupCallbacks(void)
{
	int thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, nullptr);
	if (thid >= 0) {
		sceKernelStartThread(thid, 0, nullptr);
	}
	return thid;
}

void PSP_Init()
{
	SetupCallbacks();
	scePowerSetClockFrequency(333, 333, 166);
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
}

// ==============================
// PSP entry point
// ==============================
int main(int argc, char* argv[])
{
	PSP_Init();

	// Call the "real" OpenTTD main function
	extern int openttd_main(int argc, char* argv[]);
	return openttd_main(argc, argv);
}

#endif /* PSP */
