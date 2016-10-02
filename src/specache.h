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

#include "common.h"

#include "imlib/specs.h"

class SpecDirCache
{
    class FileNode
    {
        friend class SpecDirCache;

    public:
        FileNode(String const filename, SpecDir *dir)
        {
            m_name = filename;
            m_sd = dir;
            m_next = m_left = m_right = nullptr;
        }

    private:
        FileNode *m_left, *m_right, *m_next;
        SpecDir *m_sd;
        String m_name;
        uint16_t m_size;
    }
    *m_root, *m_list;

public:
    SpecDirCache() : m_root(nullptr), m_size(0) { }
    ~SpecDirCache();

    SpecDir *GetSpecDir(String const filename, bFILE *fp = nullptr);
    void Load(bFILE *fp);
    void Save(bFILE *fp);
    void Clear();

private:
    void ClearNode(FileNode *f);

    uint16_t m_size;
};

extern SpecDirCache g_sd_cache;

