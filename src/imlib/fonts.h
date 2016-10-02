/*
 *  Abuse — dark 2D side-scrolling platform game
 *  Copyright © 1995 Crack dot Com
 *  Copyright © 2005—2016 Sam Hocevar <sam@hocevar.net>
 *
 *  This software was released into the Public Domain. As with most public
 *  domain software, no warranty is made or implied by Crack dot Com, by
 *  Jonathan Clark, or by Sam Hocevar.
 */

#pragma once

#include "imlib/image.h"
#include "imlib/transimage.h"

class JCFont
{
public:
    JCFont(AImage *letters);
    ~JCFont();

    void PutChar(AImage *screen, ivec2 pos, char ch, int color = -1);
    void PutString(AImage *screen, ivec2 pos, String const &st, int color = -1);
    ivec2 Size() const { return m_size; }

private:
    ivec2 m_size;
    TransImage *m_data[256];
};

