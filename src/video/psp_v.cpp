#include "../stdafx.h"

#ifdef PSP

#include "../openttd.h"
#include "../gfx_func.h"
#include "../blitter/factory.hpp"
#include "../core/random_func.hpp"
#include "../progress.h"
#include "psp_v.h"

#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspctrl.h>

static FVideoDriver_PSP iFVideoDriver_PSP;

#define PSP_LINE_SIZE 512
#define PSP_SCREEN_WIDTH 480
#define PSP_SCREEN_HEIGHT 272

static unsigned int __attribute__((aligned(16))) list[262144];

std::optional<std::string_view> VideoDriver_PSP::Start(const StringList &param)
{
    this->_screen_width = PSP_SCREEN_WIDTH;
    this->_screen_height = PSP_SCREEN_HEIGHT;

    sceGuInit();
    sceGuStart(GU_DIRECT, list);
    sceGuDrawBuffer(GU_PSM_8888, nullptr, PSP_LINE_SIZE);
    sceGuDispBuffer(PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, (void *)0x88000, PSP_LINE_SIZE);
    sceGuDepthBuffer((void *)0x110000, PSP_LINE_SIZE);
    sceGuOffset(2048 - (PSP_SCREEN_WIDTH / 2), 2048 - (PSP_SCREEN_HEIGHT / 2));
    sceGuViewport(2048, 2048, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT);
    sceGuScissor(0, 0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT);
    sceGuEnable(GU_SCISSOR_TEST);
    sceGuFinish();
    sceGuSync(0, 0);
    sceDisplayWaitVblankStart();
    sceGuDisplay(GU_TRUE);

    return std::nullopt;
}

void VideoDriver_PSP::Stop()
{
    sceGuTerm();
}

void VideoDriver_PSP::MakeDirty(int left, int top, int width, int height)
{
}

bool VideoDriver_PSP::ChangeResolution(int w, int h)
{
    return false;
}

bool VideoDriver_PSP::ToggleFullscreen(bool fullscreen)
{
    return false;
}

bool VideoDriver_PSP::AfterBlitterChange()
{
    return true;
}

void VideoDriver_PSP::PopulateSystemSprites()
{
}

bool VideoDriver_PSP::Paint()
{
    return true;
}

void VideoDriver_PSP::MainLoop()
{
    for (;;)
    {
        InteractiveRandom();
        if (_exit_game)
            break;

        this->Paint();
        sceDisplayWaitVblankStart();
        sceGuSwapBuffers();
    }
}

#endif /* PSP */
