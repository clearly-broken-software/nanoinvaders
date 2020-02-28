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

#include "NanoInvadersUI.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

NanoInvadersUI::NanoInvadersUI()
    : UI(747, 800)

{
    Window &pw = getParentWindow();
    pw.addIdleCallback(this);
    fFont = createFontFromFile("sans", "/usr/share/fonts/truetype/ttf-dejavu/DejaVuSans.ttf");
    fInvaders = createFontFromMemory("pixel-invaders", fonts::invaders_from_space_ttf, fonts::invaders_from_space_ttf_size, false);
    cabinet = createImageFromRGBA(Artwork::NanoInvadersCabinetWidth, Artwork::NanoInvadersCabinetHeight, (const uchar *)Artwork::NanoInvadersCabinetData, 1);
    backdrop = createImageFromRGBA(Artwork::NanoInvadersBackDropWidth, Artwork::NanoInvadersBackDropHeight, (const uchar *)Artwork::NanoInvadersBackDropData, 1);
    initSprites();
    
    newTime = std::chrono::high_resolution_clock::now();
    oldTime = newTime;
}

void NanoInvadersUI::parameterChanged(uint32_t index, float value)
{
    repaint();
}

void NanoInvadersUI::onNanoDisplay()
{
    // draw backdrop
    Paint p;
    p = imagePattern(0, 0, Artwork::NanoInvadersBackDropWidth, Artwork::NanoInvadersBackDropHeight, 0, backdrop, 1.0f);
    beginPath();
    fillPaint(p);
    rect(0, 0, Artwork::NanoInvadersBackDropWidth, Artwork::NanoInvadersBackDropHeight);
    fill();
    closePath();

    // draw aliens
    textAlign(ALIGN_TOP);
    beginPath();
    fontFaceId(fInvaders);
    fontSize(20);
    fillColor(0xcc, 0xcc, 0xcc);
    for (uint i = 0; i < 55; i++)
    {
        if (aliens[i].active)
        {
            const char *alien = aliens[i].sprite;
            int state = aliens[i].state;
            if (state == splash)
            {
                aliens[i].countdown--;
                if (aliens[i].countdown <= 0)
                {
                    aliens[i].active = false;
                }
            }
            else
            {
                state = swapAlien ? sprite1 : sprite2;
            }

            int x = aliens[i].x;
            int y = aliens[i].y;
            text(x, y, alien + state, alien + state + 1);
        }
    }

    closePath();

    // draw canon
    fontSize(canon.fontSize);
    text(canon.x, canon.y, canon.sprite, NULL);

    // draw playerbullit
    if (playerBullit.active)
    {
        beginPath();
        fillColor(0xcc, 0xcc, 0xcc);
        rect(playerBullit.x, playerBullit.y, playerBullit.w, playerBullit.h);
        fill();
        closePath();
    }

    // draw cabinet
    p = imagePattern(0, 0, (float)Artwork::NanoInvadersCabinetWidth, Artwork::NanoInvadersCabinetHeight, 0, cabinet, 1.0f);
    beginPath();
    fillPaint(p);
    rect(0, 0, Artwork::NanoInvadersCabinetWidth, Artwork::NanoInvadersCabinetHeight);
    fill();
    closePath();
}

void NanoInvadersUI::idleCallback()
{
    newTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed_seconds = newTime - oldTime;
    if ((elapsed_seconds.count() > alienMoveSpeed))
    {
        swapAlien = !swapAlien;
        moveAliens();
        oldTime = newTime;
    }
    if (playerBullit.active)
        movePlayerBullit();
    repaint();
}

bool NanoInvadersUI::onKeyboard(const KeyboardEvent &ev)
{
    if (ev.key == 32 && ev.press && !playerBullit.active)
    {
        playerBullit.x = canon.x + 12;
        playerBullit.y = canon.y - 43;
        playerBullit.active = true;
    }
    return false;
}

bool NanoInvadersUI::onSpecial(const SpecialEvent &ev)
{
    const uint key = ev.key;
    const bool press = ev.press;
    if (key == 15)
        canon.move = press ? right : none;
    if (key == 13)
        canon.move = press ? left : none;

    if (canon.move)
        moveCanon();

    return false;
}

void NanoInvadersUI::initAliens()
{
    const uint offsetx = 40; // distance between aliens
    const uint offsety = 30;
    const uint rows = 5;
    const uint columns = 11;
    const char *alienArray[]{
        "DEZ",
        "CBZ",
        "CBZ",
        "FGZ",
        "FGZ"};
    for (uint i = 0; i < rows; i++)
    {
        for (uint j = 0; j < columns; j++)
        {
            const uint index = i * columns + j;
            aliens[index].index = index;
            aliens[index].active = true;
            aliens[index].x = j * offsetx + SCREENLEFT;
            aliens[index].y = i * offsety + 330;
            aliens[index].state = sprite1;
            aliens[index].sprite = alienArray[i];
        }
    }
    // set size
    for (uint i = 0; i < 11; i++)
    {
        aliens[i].w = 18;
        aliens[i].h = 18;
    }
    for (uint i = 0; i < 22; i++)
    {
        aliens[i + 11].w = 24;
        aliens[i + 11].h = 18;
    }
    for (uint i = 0; i < 22; i++)
    {
        aliens[i + 33].w = 26;
        aliens[i + 33].h = 18;
    }
}

void NanoInvadersUI::initSprites()
{
    initAliens();

    canon.index = 100;
    canon.sprite = "W";
    canon.x = (SCREENRIGHT - SCREENLEFT) /2 ;
    canon.y = SCREENBOTTOM - 16;
    canon.w = 26;
    canon.h = 19;
    canon.fontSize = 32;
    canon.move = none;

    alienBullit.active = false;
    alienBullit.index = 200;
    alienBullit.sprite = "Y";

    playerBullit.index = 300;
    playerBullit.active = false;
    playerBullit.sprite = ""; // no character, we'll draw a rect
    playerBullit.state = sprite1;
    playerBullit.w = 2;
    playerBullit.h = 24;
}

void NanoInvadersUI::moveAliens()
{

    for (int i = 0; i < ALIENS; i++)
    {
        if (!aliens[i].active)
            continue;

        if (aliens[i].x < SCREENLEFT +5 )
        {
            alienMoveX = 5;
            if (moveAlienDown)
            {
                moveAliensDown();
                return;
            }
        }
        if (aliens[i].x + aliens[i].w > SCREENRIGHT)
        {
            alienMoveX = -5;
            if (moveAlienDown)
            {
                moveAliensDown();
                return;
            }
        }
    }

    for (int i = 0; i < ALIENS; i++)
    {
        if (aliens[i].state == splash)
            continue;

        aliens[i].x += alienMoveX;
    }
    moveAlienDown = true;
}

void NanoInvadersUI::moveAliensDown()
{
    for (int i = 0; i < ALIENS; i++)
    {
        aliens[i].y += 5;
    }
    moveAlienDown = false;
    alienMoveSpeed = fmax(0.04, alienMoveSpeed - 0.075f); // max speed = 0.04 == 25fps
   
}

void NanoInvadersUI::moveCanon()
{

    canon.x += canon.move;
    // clamp
    const int hi = SCREENRIGHT - canon.w;
    canon.x = (canon.x < SCREENLEFT) ? SCREENLEFT : (hi < canon.x) ? hi : canon.x;
    repaint();
}

void NanoInvadersUI::movePlayerBullit()
{
    playerBullit.y -= playerBullitSpeed;
    if (playerBullit.y < SCREENTOP)
        playerBullit.active = false;

    const int bx = playerBullit.x;
    const int by = playerBullit.y;
    const int bw = playerBullit.w;
    const int bh = playerBullit.h;
    // collision detection
    for (int i = 0; i < ALIENS; i++)
    {
        if (aliens[i].active)
        {
            const int ax = aliens[i].x;
            const int ay = aliens[i].y;
            const int aw = aliens[i].w;
            const int ah = aliens[i].h;
            if (bx >= ax && bx <= ax + aw && by >= ay && by <= ay + ah)
            {
                 aliens[i].state = splash;
                playerBullit.active = false;
            }
        }
    }
}

END_NAMESPACE_DISTRHO
