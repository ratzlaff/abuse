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

#include <stdio.h>
#include <stdlib.h>

#include "imlib/specs.h"

class point_list
{
public :
  unsigned char tot;
  unsigned char *data;
  point_list(unsigned char how_many, unsigned char *Data);
  point_list() { tot=0; data=NULL; }
  point_list(bFILE *fp);
  void save(bFILE *fp);
  long size() { return 1+2*tot; }
  point_list *copy() { return new point_list(tot,data); }
  ~point_list() { if (tot) { free(data); } }
};

