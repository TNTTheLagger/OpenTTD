#ifndef VIDEO_PSP_H
#define VIDEO_PSP_H

#include "../driver.h"
#include "../core/geometry_type.hpp"

class VideoDriver_PSP : public VideoDriver
{
public:
    std::optional<std::string_view> Start(const StringList &param) override;
    void Stop() override;
    void MakeDirty(int left, int top, int width, int height) override;
    void MainLoop() override;
    bool ChangeResolution(int w, int h) override;
    bool ToggleFullscreen(bool fullscreen) override;
    bool AfterBlitterChange() override;
    std::string_view GetName() const override { return "psp"; }
    bool UseSystemCursor() override { return false; }
    void PopulateSystemSprites() override;
    bool HasGUI() const override { return true; }

private:
    void *_psp_screen;
    int _screen_width;
    int _screen_height;
    bool Paint();
};

class FVideoDriver_PSP : public DriverFactoryBase
{
public:
    FVideoDriver_PSP() : DriverFactoryBase(Driver::DT_VIDEO, 10, "psp", "PSP Video Driver") {}
    Driver *CreateInstance() const override { return new VideoDriver_PSP(); }
};

#endif /* VIDEO_PSP_H */
