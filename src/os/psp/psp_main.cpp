#ifdef PSP

#include <pspkernel.h>
#include <pspdebug.h>
#include <psppower.h>
#include <pspctrl.h>
#include <vector>
#include <string>
#include <span>

// Use the PSP module info macros
PSP_MODULE_INFO("OpenTTD", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-256);

// Forward declaration of the OpenTTD core main function.
// This is defined in the core OpenTTD source (openttd_main).
// See openttd.h for its signature. :contentReference[oaicite:1]{index=1}
extern "C" int openttd_main(std::span<const std::string_view> arguments);

// Callback and init functions (you may already have these in psp.cpp)
extern int SetupCallbacks();

// The actual main() that PSP expects.
int main(int argc, char* argv[]) {
	// Initialize debug output (optional)
	pspDebugScreenInit();

	// Run PSP callbacks for exit and power
	SetupCallbacks();

	// Lock PSP clock to max speed
	scePowerSetClockFrequency(333, 333, 166);

	int result = openttd_main({});

	// Return back to PSP (exit)
	sceKernelExitGame();
	return result;
}

#endif // PSP
