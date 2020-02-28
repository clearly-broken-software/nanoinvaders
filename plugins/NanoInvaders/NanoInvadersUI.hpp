/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2019 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef NANOINVADERS_UI_HPP
#define NANOINVADERS_UI_HPP

#include "DistrhoPluginInfo.h"
#include "DistrhoUI.hpp"
#include "Window.hpp"
#include "Artwork.hpp"
#include "Resources/font.hpp"
#include <chrono>
#include <algorithm>

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

class NanoInvadersUI : public UI,
                       public IdleCallback
{
public:
    NanoInvadersUI();

protected:
    void parameterChanged(uint32_t index, float value) override;
    void onNanoDisplay() override;
    void idleCallback() override;
    bool onKeyboard(const KeyboardEvent &ev) override;
    bool onSpecial(const SpecialEvent &) override;

private:
    void initSprites();
    void initAliens();
    void moveAliens();
    void moveAliensDown();
    void moveCanon();
    void movePlayerBullit();
    enum State
    {
        sprite1,
        sprite2,
        splash
    };
   
    enum Movement
    {
        left = -5,
        none = 0,
        right = 5
    };
   
    struct Sprite
    {
        uint index;
        int x, y;
        uint w, h, fontSize;
        const char *sprite;
        bool active;
        int countdown = 25;
        State state = sprite1;
        Movement move = none;
    };
#define ALIENS 55
#define SCREENLEFT 90
#define SCREENRIGHT 684
#define SCREENBOTTOM 600
#define SCREENTOP 255
    struct Sprite aliens[ALIENS];
    Sprite canon, alienBullit, playerBullit;
    bool swapAlien;
    bool moveAlienDown = true;
    int alienMoveX = 5;
    float alienMoveSpeed = .5f;
    const uint playerBullitSpeed = 5;
    

    // UI stuff
    FontId fFont;
    FontId fInvaders;
    NanoImage backdrop, cabinet;

    // Timers
    std::chrono::high_resolution_clock::time_point oldTime, newTime;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NanoInvadersUI)
};

/* ------------------------------------------------------------------------------------------------------------
 * UI entry point, called by DPF to create a new UI instance. */

UI *createUI()
{
    return new NanoInvadersUI();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
#endif
