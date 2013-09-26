/*
 *  Abuse - dark 2D side-scrolling platform game
 *  Copyright (c) 2001 Anthony Kruize <trandor@labyrinth.net.au>
 *  Copyright (c) 2005-2013 Sam Hocevar <sam@hocevar.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 */

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "common.h"

#include "imlib/event.h"
#include "imlib/video.h"
#include "imlib/filter.h"

//
// Constructor
//
EventHandler::EventHandler(AImage *screen, Palette *pal)
{
    ASSERT(screen);
    ASSERT(pal);

    m_pending = 0;
    m_screen = screen;

    // Mouse stuff
    uint8_t mouse_sprite[]=
    {
        0, 2, 0, 0, 0, 0, 0, 0,
        2, 1, 2, 0, 0, 0, 0, 0,
        2, 1, 1, 2, 0, 0, 0, 0,
        2, 1, 1, 1, 2, 0, 0, 0,
        2, 1, 1, 1, 1, 2, 0, 0,
        2, 1, 1, 1, 1, 1, 2, 0,
        0, 2, 1, 1, 2, 2, 0, 0,
        0, 0, 2, 1, 1, 2, 0, 0,
        0, 0, 2, 1, 1, 2, 0, 0,
        0, 0, 0, 2, 2, 0, 0, 0
    };

    Filter f;
    f.Set(1, pal->FindBrightest(1));
    f.Set(2, pal->FindDarkest(1));
    AImage *im = new AImage(ivec2(8, 10));
    memcpy(im->scan_line(0), mouse_sprite, 8 * 10);
    f.Apply(im);

    m_sprite = new ASprite(screen, im, ivec2(100, 100));
    m_pos = screen->Size() / 2;
    m_center = ivec2(0, 0);
    m_button = 0;
}

//
// Destructor
//
EventHandler::~EventHandler()
{
    ;
}

void EventHandler::Get(Event &ev)
{
    // Sleep until there are events available
    for (Timer t; !m_pending; t.Wait(0.001))
        IsPending();

    // Return first queued event if applicable
    if (m_events.TryPop(ev))
        return;

    // Return an event from the platform-specific system
    SysEvent(ev);
}

//
// flush_screen()
// Redraw the screen
//
void EventHandler::flush_screen()
{
    update_dirty(main_screen);
}

